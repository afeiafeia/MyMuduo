#ifndef LOG_EVENT_WARP_H
#define LOG_EVENT_WARP_H
#include <memory>
#include "LogEvent.h"
#include "Logger.h"
namespace afa
{
    class LogEventWarp
    {
    public:
        typedef std::shared_ptr<LogEventWarp> Ptr;

    private:
        LogEvent::Ptr m_event; //日志事件
        Logger::Ptr   m_logger;//日志器
    public:
        /**
         * @brief   构造函数
         * @param   logger      参数描述
         * @param   event       参数描述
         * @author  zhangafei
         * @date    2022-10-06
         */
        LogEventWarp(Logger::Ptr logger,LogEvent::Ptr event);

        /**
         * @brief   利用RAII机制，在析构时进行日志的输出
         * @author  zhangafei
         * @date    2022-10-06
         */
        ~LogEventWarp();

        std::stringstream& GetSS(){return m_event->GetSS();}
    };
}



#endif