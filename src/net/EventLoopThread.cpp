#include "EventLoopThread.h"
#include "../base/MutexLockGuard.h"
#include "../base/Log.h"
#include "EventLoopThreadPool.h"
namespace afa
{
    static Logger::Ptr log = LOG_ROOT();
    EventLoopThread::EventLoopThread(EventLoopThreadPool* pool)
    :loop_(nullptr)
    ,m_pool(pool)
    ,thread_(std::bind(&EventLoopThread::ThreadFunc,this),"EventLoopThread")
    ,mutex_()
    ,cond_(mutex_)
    ,m_quitCB(std::bind(&EventLoopThreadPool::EraseLoop,pool,std::placeholders::_1))
    {

    }

    EventLoopThread::~EventLoopThread()
    {
        if(loop_!=nullptr)
        {
            thread_.Join();
        }
    }

    //此函数由主线程调用
    EventLoop* EventLoopThread::StartLoop()
    {
        thread_.Start();
        {
            MutexLockGuard lock(&mutex_);
            while(loop_==nullptr)
            {
                //一直到线程ThreadFunc真正跑起来
                cond_.Wait();
            }

        }
        return loop_;
    }

    //工作线程
    void EventLoopThread::ThreadFunc()
    {
        EventLoop loop;
        {
            MutexLockGuard lock(&mutex_);
            loop_ = &loop;
            cond_.Signal();
        }
        loop_->Loop();
        m_quitCB(&loop);
        MutexLockGuard lock(&mutex_);
        loop_=nullptr;
    }

}