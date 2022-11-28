#include "EventLoop.h"
#include "../base/MutexLockGuard.h"
#include <sys/eventfd.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "EpollPoller.h"
#include "Channel.h"
#include "../base/Log.h"

//获取日志输出器
static afa::Logger::Ptr logger = LOG_ROOT();
namespace afa
{
    __thread EventLoop* t_loopInThisThread = nullptr;
    __thread char t_thread_name[256]="Default";
    int CreateEventfd()
    {
        int evefd = eventfd(0,EFD_NONBLOCK|EFD_CLOEXEC);
        if(evefd<0)
        {
            abort();
        }
        LOG_DEBUG(logger)<<"Creating eventfd: "<<evefd;
        return evefd;
    }

    EventLoop::EventLoop()
    :m_quit(false)
    ,m_threadId(CurrentThread::tid())
    ,m_wakeId(CreateEventfd())
    ,m_isDoPendingFun(false)
    ,m_spWakeUpChannel(new Channel(m_wakeId,this))
    ,m_spEpoll(new EpollPoller(this))
    ,m_mutex(MutexLock())
    ,m_timer_manager(this,3000)
    {
        //LOG_DEBUG(logger)<<"EventLoop: "<<this<<" is created in thread "<<CurrentThread::tid();

        t_loopInThisThread = this;
        m_spWakeUpChannel->SetReadHandle(std::bind(&EventLoop::HandleRead,this));
        m_spWakeUpChannel->EnableReading();
    }

    EventLoop::~EventLoop()
    {
        //LOG_DEBUG(logger)<< "EventLoop: " << this << " of thread " << m_threadId
            //<< "is destructed in thread " << CurrentThread::tid();
        //m_spWakeUpChannel->DisableAll();

        m_spWakeUpChannel->Remove();
        t_loopInThisThread = nullptr;
    }

    void EventLoop::SetThreadName(const std::string &new_name)
    {
        assertInLoopThread();
        memset(t_thread_name,0,256);
        memcpy(t_thread_name,new_name.c_str(),new_name.size());
        CurrentThread::t_threadName = t_thread_name;
    }

    void EventLoop::Loop()
    {
        assertInLoopThread();
        m_quit = false;
        //LOG_INFO(logger)<<"EventLoop: "<<this<<" in thread "<<m_threadId<<" start looping!";
        TimeStamp timeout(-1);
        while(m_quit==false)
        {
            std::vector<Channel*> vctActiveChannel;
            //LOG_INFO(logger)<<"Loop Wait!";
            m_spEpoll->Poll(vctActiveChannel,timeout);
            int num = vctActiveChannel.size();
            //LOG_DEBUG(logger)<<"Active Channel Num Is: "<<num;
            for(int i=0;i<num;i++)
            {
                vctActiveChannel[i]->HandleEvent();
            }
            DoPendingFactor();
            if(m_quit==true)
            {
                break;
            }
        }
        //LOG_INFO(logger)<<"EventLoop: "<<this<<" in thread "<<m_threadId<<" stop looping!";
    }


    void EventLoop::Quit()
    {
        //LOG_DEBUG(logger)<<"EventLoop: "<<this<<" of thread "<<m_threadId
        //<<" is quited by "<<CurrentThread::t_threadName<<" with threadId "<<CurrentThread::tid();
        //此函数由主线程调用，用来使工作线程结束loop循环
        m_quit = true;
        //由于工作线程此时可能阻塞，主线程需要将其唤醒
        if(!IsInLoopThread())
        {
            WakeUp();
        }
    }

    void EventLoop::WakeUp()
    {
        uint64_t one = 1;
        int n = write(m_wakeId,(char*)(&one),sizeof(uint64_t));
        if(n!=sizeof(uint64_t))
        {
            //上次的写入还没有读取！
            LOG_ERROR(logger)<<"Failed To WakeUp!\n";
        }
        else
        {
            LOG_DEBUG(logger)<<"EventLoop: "<<this<<" of thread "<<m_threadId
            <<" is wakeup by "<<CurrentThread::t_threadName<<" with threadId "<<CurrentThread::tid();
        }
    }

    void EventLoop::HandleRead()
    {
        uint64_t one = 1;
        size_t n = read(m_wakeId,&one,sizeof(one));
        if(n!=sizeof(one))
        {
            LOG_ERROR(logger)<<"EventLoop::HandleRead() read"<<n<<"bytes instead of 8!";
        }
        else
        {
            LOG_DEBUG(logger)<<"WakeUpChannel's HandleRead:WakeUp!";
        }
    }

    void EventLoop::RunInLoop(const Functor &cb)
    {
        if(IsInLoopThread())
        {
            cb();
        }
        else
        {
            QueueInLoop(std::move(cb));
        }
    }

    void EventLoop::RunAfter(const Functor &cb,TimeStamp after)
    {
        RunAt(cb,after+TimeStamp::Now());
    }

    void EventLoop::RunAt(const Functor &cb,TimeStamp time)
    {
        Timer*  timer = new Timer(cb,time,0);
        QueueInLoop(std::bind(&EventLoop::AddTimer,this,timer));
    }

    void EventLoop::QueueInLoop(const Functor &cb)
    {
        LOG_DEBUG(logger)<<"Function is queued in vector!";
        {
            MutexLockGuard lock(&m_mutex);
            m_vctPendingFactor.emplace_back(std::move(cb));
        }
        if(!IsInLoopThread()||!m_isDoPendingFun)
        {
            WakeUp();
        }
    }

    void EventLoop::UpdateChannel(Channel* pChannel)
    {
        m_spEpoll->UpdateChannel(pChannel);
    }

     void EventLoop::RemoveChannel(Channel* pChannel)
    {
        if(pChannel==nullptr)
        {
            return;
        }
        LOG_DEBUG(logger)<<"EventLoop remove channel with fd = "<<pChannel->Getfd();
        pChannel->DisableAll();
    }

    void EventLoop::DoPendingFactor()
    {
        //当执行此函数时，其它线程可能正在向m_vctPendingFunctor中放入一些待执行的函数
        //如果直接使用m_vctPendingFunctor，可能存在的一个问题是：在该函数取数据过程（for循环中），
        //其它线程放入数据可能造成内存迁移
        //对上述问题，可以在for循环是加锁，但这样锁的粒度将很大（所有pendFun函数执行完才释放锁）
        //因此这里将原m_vctPendingFUnctor中的函数迁移出来，对迁移的目标进行处理，这样在执行pendFun函数期间，其它线程可以继续向原m_vctPendingFunctor进行放入（二者操作的是不同对象了）
        std::vector<Functor> curPendingFunctor;
        m_isDoPendingFun = true; //避免在执行此函数区间再次对函数唤醒（再次唤醒时没有必要的）
        {
            MutexLockGuard guard(&m_mutex);
            m_vctPendingFactor.swap(curPendingFunctor);
        }
        int num = curPendingFunctor.size();
        LOG_DEBUG(logger)<<"PendIngFun Num Is: "<<num;
        for(int i=0;i<num;i++)
        {
            curPendingFunctor[i]();
        }
        m_isDoPendingFun = false;
        LOG_DEBUG(logger)<<"PendIngFun Done!";
    }

    EventLoop* EventLoop::GetEventLoopOfCurThread()
    {
        return t_loopInThisThread;
    }

    void EventLoop::AddTimer(Timer* timer)
    {
        assertInLoopThread();
        m_timer_manager.AddTimer(timer);
    }

    void EventLoop::UpdateTimer(Timer* timer)
    {
        assertInLoopThread();
        m_timer_manager.UpdateTimer(timer);
    }
    void EventLoop::EraseTimer(Timer* timer)
    {
        assertInLoopThread();
        if(timer==nullptr)
        {
            return;
        }
        m_timer_manager.EraseTimer(timer);
    }
}