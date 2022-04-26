#include "LogEvent.h"
#include <stdarg.h>
namespace afa
{

    LogEvent::LogEvent(
        LogLevel::Level level,
        std::string     file_name,
        std::string     method_name,
        u_int32_t       line,
        u_int32_t       threadId,
        u_int32_t       fiberId,
        u_int64_t       time,
        std::string     thread_name)
        :m_level(level)
        ,m_file_name(file_name)
        ,m_method_name(method_name)
        ,m_line(line)
        ,m_thread_id(threadId)
        ,m_fiber_id(fiberId)
        ,m_time(time)
        ,m_thread_name(thread_name)
        {
        }

        LogEvent::~LogEvent()
        {

        }

    void LogEvent::Format(const char* format_content,...)
    {
        va_list al;
        va_start(al,format_content);
        Format(format_content,al);
        va_end(al);
    }

    void LogEvent::Format(const char* format_content,va_list va)
    {
        char* buff = nullptr;
        int len = vasprintf(&buff,format_content,va);
        if(len>0)
        {
            m_format_content = std::move(std::string(buff));
            free(buff);
        }
    }
}