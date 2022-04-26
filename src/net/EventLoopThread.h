#ifndef EVENT_LOOP_THREAD_H
#define EVENT_LOOP_THREAD_H
#include <functional>
#include "EventLoop.h"
#include "../base/Thread.h"
#include "../base/MutexLock.h"
#include "../base/Condition.h"
namespace afa
{
    class EventLoopThreadPool;
    class EventLoopThread
    {
    private:
        typedef std::function<void()> ThreadFun;
        typedef std::function<void(EventLoop*)> EraseCallBack;
        EventLoop* loop_;
        EventLoopThreadPool* m_pool;
        Thread thread_;

        MutexLock mutex_;
        Condition cond_;

        EraseCallBack   m_quitCB;

    public:
        /**
         * @brief  Construct a new Event Loop Thread object
         * @author afei
         * @date   2022-04-18
         */
        EventLoopThread(EventLoopThreadPool* pool);

        /**
         * @brief  Destroy the Event Loop Thread object
         * @author afei
         * @date   2022-04-18
         */
        ~EventLoopThread();

        EventLoop* StartLoop();

        void ThreadFunc();

        void EraseFromPool();
    };
}



#endif