#include "TimeStamp.h"
#include <sys/time.h>
#include <stddef.h>
namespace afa
{
    TimeStamp::TimeStamp()
    :m_time(Now())
    {

    }
    TimeStamp::TimeStamp(int64_t time)
    :m_time(time)
    {

    }

    TimeStamp TimeStamp::operator+(const TimeStamp &rhs)const
    {
        return TimeStamp(rhs.m_time+m_time);
    }

    bool TimeStamp::operator<(const TimeStamp &rhs)const
    {
        return m_time<rhs.m_time;
    }

    bool TimeStamp::operator>(const TimeStamp &rhs)const
    {
        return m_time>rhs.m_time;
    }


    int64_t TimeStamp::Now()
    {
        struct timeval now;
        gettimeofday(&now, NULL);

        int64_t cur = now.tv_sec*time_unit*time_unit;
        cur+=now.tv_usec;
        return cur;
    }

    int64_t TimeStamp::GetTime()const
    {
        return m_time;
    }

    void TimeStamp::SetNewTime(int64_t new_time)
    {
        m_time = new_time;
    }

}