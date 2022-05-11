#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include "../base/CurrentThread.h"
#include "../base/MutexLock.h"
#include "TimerManager.h"
#include <sys/wait.h>
#include <functional>
#include <vector>
#include <assert.h>
#include <memory>
#include <atomic>
namespace afa
{
    class Channel;
    class EpollPoller;
    
    class EventLoop
    {
    public:
        typedef std::function<void()> Functor;
        typedef std::shared_ptr<Channel> SP_Channel;
        EventLoop();
        ~EventLoop();


        void SetThreadName(const std::string &new_name);
        /**
         * @brief  事件循环
         * @author afei
         * @date   2022-04-06
         */
        void Loop();

        /**
         * @brief  使事件循环终止
         * @author afei
         * @date   2022-04-06
         */
        void Quit();

        /**
         * @brief  唤醒阻塞在io复用上的线程
         * @author afei
         * @date   2022-04-06
         */
        void WakeUp();

        /**
         * @brief  判断执行IsInLoopThread的线程是否为创建该loop的线程
         * @author afei
         * @date   2022-03-27
         * @return true 
         * @return false 
         */
        bool IsInLoopThread() {return m_threadId==CurrentThread::tid();}

        /**
         * @brief  断言：判断是不是在当前线程调用的该函数
         * @author afei
         * @date   2022-04-18
         */
        void assertInLoopThread(){assert(IsInLoopThread());}
        
        /**
         * @brief  在loop所在线程运行传入的函数（如果当前不造loop所在线程，则将函数排入待执行队列并唤醒loop线程）
         * @param  cb               
         * @author afei
         * @date   2022-04-18
         */
        void RunInLoop(const Functor &cb);

        /**
         * @brief  将传入函数排入待执行队列并唤醒loop线程
         * @param  cb               
         * @author afei
         * @date   2022-04-18
         */
        void QueueInLoop(const Functor &cb);

        /**
         * @brief  更新epoll对于spChannel的监听状态
         * @param  spChannel        
         * @author afei
         * @date   2022-04-18
         */
        void UpdateChannel(Channel* pChannel);

        /**
         * @brief  epoll中移除对于spChannel的监听
         * @param  spChannel        
         * @author afei
         * @date 2022-04-18
         */
        void RemoveChannel(Channel* pChannel);

        /**
         * @brief  执行待执行队列中的函数
         * @author afei
         * @date   2022-04-18
         */
        void DoPendingFactor();

        /**
         * @brief  读取事件描述符（作为时间描述符的读回调函数）
         * @author afei
         * @date   2022-04-18
         */
        void HandleRead();

        /**
         * @brief  获取当前线程的loop
         * @author afei
         * @date   2022-04-18
         * @return EventLoop* 
         */
        static EventLoop* GetEventLoopOfCurThread();



        void AddTimer(Timer* timer);

        void UpdateTimer(Timer* timer);

        void EraseTimer(Timer* timer);
    private:
        volatile std::atomic<bool>            m_quit;              //loop是否停止
        const pid_t                  m_threadId;          //线程id
        int                          m_wakeId;            //事件描述符，用于唤醒工作线程（通过向描述符写入数据的方式）
        bool                         m_isDoPendingFun;    //是否正在执行DoPendingFunctor,用于判断，避免在执行此函数区间再次对函数唤醒（再次唤醒时没有必要的）
        std::unique_ptr<Channel>     m_spWakeUpChannel;   //事件描述符对应的channel
        std::vector<Functor>         m_vctPendingFactor;  //待执行函数队列
        std::unique_ptr<EpollPoller> m_spEpoll;           //IO复用
        MutexLock                    m_mutex;             //锁

        TimerManager                 m_timer_manager;

    };
}

#endif