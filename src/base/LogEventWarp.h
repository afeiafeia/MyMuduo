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
        LogEvent::Ptr m_event;
        Logger::Ptr   m_logger;
    public:
        LogEventWarp(Logger::Ptr logger,LogEvent::Ptr event);

        ~LogEventWarp();

        std::stringstream& GetSS(){return m_event->GetSS();}
    };
}



#endif