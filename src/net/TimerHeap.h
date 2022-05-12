#ifndef TIMER_HEAP_H
#define TIMER_HEAP_H

#include <vector>
#include "Timer.h"
#include "../base/MutexLock.h"
namespace afa
{
    //定时器的增删查改都在loop中进行，因此不使用锁
    class TimerHeap
    {
    public:
        TimerHeap();
        ~TimerHeap();

        void AddTimer(Timer* addTimer);

        //删除过期的定时器
        void DelTimer();

        void UpdateTimer(Timer* timer);

        void EraseTimer(Timer* timer);
        
    private:
        void PopTimer();
        int Search(Timer* timer);
    private:
        std::vector<Timer*> m_vctTimer;
    };
}

#endif