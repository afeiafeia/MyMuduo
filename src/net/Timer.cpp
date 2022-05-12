#include "Timer.h"
#include "TcpConnection.h"
namespace afa
{
    Timer::Timer(const TimerCallBack &cb_fun,TimeStamp time)
    :m_expire(time)
    ,m_callback(cb_fun)
    {

    }

    Timer::~Timer()
    {
        if(m_callback)
        {
            m_callback();
        }
    }
}