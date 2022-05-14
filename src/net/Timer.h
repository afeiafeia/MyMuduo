#ifndef TIMER_H
#define TIMER_H
#include <memory>
#include <functional>
#include "../base/TimeStamp.h"
namespace afa
{
    class TcpConnection;
    class Timer
    {
        public:
        typedef std::function<void(void)> TimerCallBack;
    public:
        Timer(const TimerCallBack &cb_fun,TimeStamp time,int interval);
        ~Timer();

        TimeStamp GetTime()
        {
            return m_expire;
        }

        void SetTime(TimeStamp new_expire)
        {
            m_expire = new_expire;
        }

        bool ReStart();

        TimerCallBack                m_callback; //过期时执行的函数

    private:
        TimeStamp                    m_expire;   //过期时间
        int                          m_interval;//周期
    };
}


#endif