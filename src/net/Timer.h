#ifndef TIMER_H
#define TIMER_H
#include <memory>
#include <functional>
namespace afa
{
    class TcpConnection;
    class Timer
    {
        public:
        typedef std::function<void(void)> TimerCallBack;
    public:
        Timer(const TimerCallBack &cb_fun,int64_t time);
        ~Timer();

        int64_t GetTime()
        {
            return m_expire;
        }

        void SetTime(int64_t new_expire)
        {
            m_expire = new_expire;
        }

    private:
        int64_t                      m_expire;   //过期时间
        TimerCallBack                m_callback; //过期时执行的函数
    };
}


#endif