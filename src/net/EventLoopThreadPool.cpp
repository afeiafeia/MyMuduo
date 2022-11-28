
#include "EventLoopThreadPool.h"
#include "../base/Log.h"
#include "../base/MutexLockGuard.h"
namespace afa
{
    static Logger::Ptr log = LOG_ROOT();
    EventLoopThreadPool::EventLoopThreadPool(EventLoop* loop,int numThread)
    :m_loop(loop)
    ,m_threadNum(numThread)
    ,m_nextIndex(0)
    ,m_isStarted(false)
    {

    }

    EventLoopThreadPool::~EventLoopThreadPool()
    {
        
    }

    void EventLoopThreadPool::Start()
    {
        m_loop->assertInLoopThread();//保证都在主线程调用该函数
        m_isStarted = true;
        for(int i=0;i<m_threadNum;i++)
        {
            std::shared_ptr<EventLoopThread> cur(new EventLoopThread(this));
            m_vctEventloopThreads.push_back(cur);
            m_vctLoops.push_back(cur->StartLoop());
        }
    }

    EventLoop* EventLoopThreadPool::GetNextLoop()
    {
        //如果loops中所指EventLoop离开声明周期了，此处如何的值？？？
        m_loop->assertInLoopThread();//保证都在主线程调用该函数
        EventLoop* curLoop = m_loop;
        MutexLockGuard guard(&m_mutex);
        if(!m_vctLoops.empty())
        {
            curLoop = m_vctLoops[m_nextIndex];
            m_nextIndex = (m_nextIndex+1)%m_threadNum;
            LOG_DEBUG(log)<<"GetNextLoop Is "<<curLoop<<"   "<<"at index: " <<m_nextIndex;
        }
        return curLoop;
    }

    void EventLoopThreadPool::EraseLoop(EventLoop* loop)
    {
        for(int i=0;i<m_vctLoops.size();i++)
        {
            if(m_vctLoops[i]==loop)
            {
                MutexLockGuard guard(&m_mutex);
                if(m_nextIndex>i)
                {
                    --m_nextIndex;
                }
                m_vctLoops.erase(m_vctLoops.begin()+i);
                m_threadNum--;
                break;
            }
        }
    }

}