#include "Timer.h"
#include "TcpConnection.h"
namespace afa
{
    Timer::Timer(const TimerCallBack &cb_fun,TimeStamp time,int interval)
    :m_expire(time)
    ,m_callback(cb_fun)
    ,m_interval(interval)
    {

    }

    Timer::~Timer()
    {

    }

    bool Timer::ReStart()
    {
        if(m_interval>0)
        {
            m_expire = TimeStamp::Now()+m_interval;
        }
        else
        {
            m_expire = TimeStamp(0);
        }
        return m_interval>0;
    }
}