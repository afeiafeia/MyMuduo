#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include "TimerHeap.h"
namespace afa
{
    class EventLoop;
    class Channel;
    class TimerManager
    {
    public:
        TimerManager(EventLoop* loop,int time_slot);
        ~TimerManager();

        void AddTimer(Timer* iAddTimer);

        void UpdateTimer(Timer* timer);

        void EraseTimer(Timer* timer);

        //可读时删除过期定时器
        void HandleRead();

        void ResetTimerFd(TimeStamp periodic,bool oneshot=true);
    private:
        EventLoop*               m_loop;
        int                      m_time_slot;//每隔m_time_slot毫秒时间触发一次可读事件
        TimerHeap                m_timer_heap;//时间堆：管理定时器
        int                      m_timerfd;//时间事件描述符
        std::unique_ptr<Channel> m_timer_channel;//时间时间描述符对应的Channel

    };
}

#endif