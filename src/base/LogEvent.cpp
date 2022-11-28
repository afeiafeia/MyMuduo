#include "LogEvent.h"
#include <stdarg.h>
namespace afa
{

    LogEvent::LogEvent(
        LogLevel::Level level,
        std::string     fileName,
        std::string     methodName,
        uint32_t        line,
        uint32_t        threadId,
        uint32_t        fiberId,
        uint64_t        time,
        std::string     threadName)
        :m_level(level)
        ,m_fileName(fileName)
        ,m_methodName(methodName)
        ,m_line(line)
        ,m_threadId(threadId)
        ,m_fiberId(fiberId)
        ,m_time(time)
        ,m_threadName(threadName)
        {
        }

        LogEvent::~LogEvent()
        {

        }

    void LogEvent::Format(const char* formatContent,...)
    {
        va_list al;
        va_start(al,formatContent);
        Format(formatContent,al);
        va_end(al);
    }

    void LogEvent::Format(const char* formatContent,va_list va)
    {
        char* buff = nullptr;
        int len = vasprintf(&buff,formatContent,va);
        if(len>0)
        {
            m_formatContent = std::move(std::string(buff));
            m_contentStream<<m_formatContent;
            free(buff);
        }
    }
}