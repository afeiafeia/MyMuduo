#ifndef EVENT_LOOP_THREAD_POOL_H
#define EVENT_LOOP_THREAD_POOL_H
#include "EventLoop.h"
#include "EventLoopThread.h"
#include <vector>
#include <memory>
#include <bits/shared_ptr.h>
#include "../base/MutexLock.h"
namespace afa
{
    class EventLoopThreadPool
    {
    public:

    private:
        EventLoop* m_loop;
        int m_threadNum;
        int m_nextIndex;
        bool m_isStarted;
        std::vector<std::shared_ptr<EventLoopThread> > m_vctEventloopThreads;
        std::vector<EventLoop*> m_vctLoops;
        MutexLock m_mutex;

    public:
        EventLoopThreadPool(EventLoop* loop,int numThread=10);
        ~EventLoopThreadPool();

        void Start();

        EventLoop* GetNextLoop();

        void EraseLoop(EventLoop* loop);

    };
}




#endif