#include "LogEventWarp.h"

namespace afa
{
    LogEventWarp::LogEventWarp(Logger::Ptr logger,LogEvent::Ptr event)
    :m_event(event)
    ,m_logger(logger)
    {

    }

    LogEventWarp::~LogEventWarp()
    {
        m_logger->Log(m_event->GetLogLevel(),m_event);
    }

    
}