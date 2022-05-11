#include "../base/Log.h"
#include "TimerManager.h"
#include "Channel.h"
#include <sys/timerfd.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
namespace afa
{
    static Logger::Ptr logger = LOG_ROOT();
    int CreateTimerfd()
    {
        int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                 TFD_NONBLOCK | TFD_CLOEXEC);
        if (timerfd < 0)
        {
          LOG_ERROR(logger) << "Failed in timerfd_create";
        }
        return timerfd;
    }
    TimerManager::TimerManager(EventLoop* loop,int time_slot)
    :m_loop(loop)
    ,m_time_slot(time_slot)
    ,m_timerfd(CreateTimerfd())
    ,m_timer_channel(new Channel(m_timerfd,loop))
    {
        m_timer_channel->SetReadHandle(std::bind(&TimerManager::HandleRead,this));
        m_timer_channel->EnableReading();

        struct timeval cur;
        gettimeofday(&cur,NULL);
        cur.tv_sec +=m_time_slot;
        cur.tv_usec +=(1000*1000*m_time_slot);
        ResetTimerFd(cur); 
    }
    TimerManager::~TimerManager()
    {

    }

    void TimerManager::AddTimer(Timer* iAddTimer)
    {
        m_timer_heap.AddTimer(iAddTimer);
    }

    void TimerManager::HandleRead()
    {
        uint64_t data;
        int n = ::read(m_timerfd,&data,sizeof(data));
        if(n!=sizeof(data))
        {
            LOG_ERROR(logger)<<"Failed in read timerfd!";
        }

        m_timer_heap.DelTimer();
    }

    void TimerManager::ResetTimerFd(struct timeval cur)
    {
        struct timeval now;
        gettimeofday(&now, NULL);

        struct timespec ts;
        ts.tv_sec = now.tv_sec-cur.tv_sec;
        ts.tv_nsec = (now.tv_usec-cur.tv_usec)*1000;

        //new_value中为设置的时间，第二个参数为0表示相对时间，相对当前调用set的时间
        //将每隔new_value.it_value的时间到期一次（m_timerfd可读）
        struct itimerspec new_value;
        struct itimerspec old_value;
        memset(&new_value,0,sizeof(new_value));
        memset(&old_value,0,sizeof(old_value));
        new_value.it_value = ts;
        new_value.it_interval=ts;//周期，每隔ts到期一次，如果该值为0，则定时器是oneshot的，只会在调用set函数后ts时间到期一次
        int ret = ::timerfd_settime(m_timerfd, 0, &new_value, &old_value);
        if (ret)
        {
          LOG_ERROR(logger) << "timerfd_settime()";
        }

    }

    void TimerManager::UpdateTimer(Timer* timer)
    {
        m_timer_heap.UpdateTimer(timer);
    }
    void TimerManager::EraseTimer(Timer* timer)
    {
        m_timer_heap.EraseTimer(timer);
    }


}