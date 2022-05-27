#ifndef LOG_APPENDER_H
#define LOG_APPENDER_H
#include <memory>
#include "LogLevel.h"
#include "LogFormatter.h"
namespace afa
{
    class LogAppender
    {
    public:
        typedef std::shared_ptr<LogAppender> Ptr;
    private:
        LogLevel::Level               m_level;       //日志级别
        std::shared_ptr<LogFormatter> m_sp_formatter;//日式格式器

    public:
        LogAppender()
        :m_level(LogLevel::DEBUG)
        ,m_sp_formatter(new LogFormatter())
        {

        }
        virtual ~LogAppender(){}

        LogFormatter::Ptr GetFormatter()
        {
            return m_sp_formatter;
        }
        void SetFormatter(LogFormatter::Ptr sp_formatter)
        {
            m_sp_formatter = sp_formatter;
        }

        LogLevel::Level GetLevel()
        {
            return m_level;
        }
        void SetLevel(LogLevel::Level level)
        {
            m_level = level;
        }

        //写日志
        virtual void Log(LogLevel::Level level,LogEvent::Ptr event)=0;
    };
}


#endif