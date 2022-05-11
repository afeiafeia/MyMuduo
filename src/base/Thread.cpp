#include "Thread.h"
#include "CurrentThread.h"
#include <assert.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>

namespace afa
{
    namespace CurrentThread
    {
        int tid();
    }
    struct ThreadData
    {
        typedef Thread::ThreadFun ThreadFun;
        std::string name;
        ThreadFun  func;
        pid_t* tid;
        Semaphore* sem;

        ThreadData(const ThreadFun& fun,const std::string &cur_name,pid_t* cur_tid,Semaphore* cur_sem)
        :name(cur_name)
        ,func(fun)
        ,tid(cur_tid)
        ,sem(cur_sem)
        {

        }

        void RunInThread()
        {
            *tid = CurrentThread::tid();
            CurrentThread::t_threadName = name.empty()?"Thread":name.c_str();
            sem->Post();//此处调用Post是为了：保证线程函数RunInThread先运行起来，然后调用post唤醒主线程，保证主线程获取该线程时，该线程已经运行起来
            func();
            CurrentThread::t_threadName = "Finished";
        }
    };
    namespace CurrentThread
    {
        __thread int  t_cachedTid;          //线程id
        __thread char t_tidString[32];      //id的字符数组（配合长度使用）
        __thread int  t_tidStringLen;       //id长度（配合字符数组使用）
        __thread const char* t_threadName = "Default"; //线程名称

        pid_t getTid()
        {
            return static_cast<pid_t>(::syscall(SYS_gettid));
        }
        pid_t getPid()
        {
            return static_cast<pid_t>(::syscall(SYS_getpid));
        }
        void cacheTid()
        {
            if(t_cachedTid==0)
            {
                t_cachedTid = getTid();
                t_tidStringLen = snprintf(t_tidString,sizeof(t_tidString),"%5d",t_cachedTid);
            }
        }
    };


    void* StartThread(void* data)
    {
        ThreadData* pData = static_cast<ThreadData*>(data);
        if(pData==nullptr)
        {
            return nullptr;
        }
        pData->RunInThread();
        //线程主逻辑结束后，data资源由当前线程释放而不是主线程
        delete pData;
        pData = nullptr;
        return nullptr;
    }

    Thread::Thread(const ThreadFun& fun,const std::string &name)
    :m_started(false)
    ,m_joined(false)
    ,m_pthread_id(0)
    ,m_tid(0)
    ,m_name(name)
    ,m_func(fun)
    ,m_sem(0)
    {
        if(m_name.empty())
        {
            m_name = "Thread";
        }
    }
    Thread::~Thread()
    {
        if(m_started&&!m_joined)
        {

            //调用线程将不等待pthreadID_的线程退出后再退出
            //对同一个线程不能即调用pthread_detach又调用pthread_join，二者只能调用一个
            pthread_detach(m_pthread_id);
        }
    }

    void Thread::Start()
    {
        assert(!m_started);
        m_started = true;
        //data资源应该在线程StartThread中释放，不在当前主线程释放；因为存在可能情况：如果主线程释放，主线程可能先运行后续内容，导致工作线程还没有使用data，data就被释放
        ThreadData* pData = new ThreadData(m_func,m_name,&m_tid,&m_sem);
        //pthread_create  成功时返回0，出错返回错误编号（正值）
        if(pthread_create(&m_pthread_id,NULL,StartThread,pData))
        {
            //线程创建失败
            m_started = false;
            delete pData;
        }
        else
        {
            //pthread_detach(pthreadID_);
            //线程创建成功
            m_sem.Wait();//主线程要先阻塞，等子线程StartThread运行起来后再返回，防止主线程后面使用子线程时，子线程还没有运行起来
        }

    }

    void Thread::Join()
    {
        //int pthread_join(pthread_t threadId,void * * ptr);
        //调用pthread_join的线程将一直阻塞，直到threadId的线程退出
        if(!m_joined)
        m_joined = true;
        pthread_join(m_pthread_id,NULL);
    }
}