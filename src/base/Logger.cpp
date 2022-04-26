#include "Logger.h"
#include "LogFileAppender.h"
#include "CurrentThread.h"
namespace afa
{
    Logger::Logger(const std::string &name,level curLevel)
    :m_name(name)
    ,m_level(curLevel)
    ,m_lock()
    {
        //默认添加一个文件日志输出器
        LogAppender::Ptr file_appender(new LogFileAppender("./"+m_name+"Log.txt"));
        m_vct_appenders.push_back(file_appender);
    }
    
    Logger::~Logger()
    {

    }



    void Logger::AddAppender(LogAppender::Ptr appender)
    {
        MutexLockGuard guard(&m_lock);
        m_vct_appenders.push_back(appender);
    }



    void Logger::Log(LogLevel::Level level,LogEvent::Ptr event)
    {
        if(level<m_level)
        {
            return;
        }

        //防止在一个线程写日志时，另外的线程正在AddAppender
        std::list<LogAppender::Ptr> vct_appenders;
        {
            MutexLockGuard guard(&m_lock);
            vct_appenders.swap(m_vct_appenders);
        }
        for(auto& it:vct_appenders)
        {
            it->Log(level,event);
        }
        {
            MutexLockGuard guard(&m_lock);
            if(!m_vct_appenders.empty())
            {
                m_vct_appenders.insert(m_vct_appenders.end(),m_vct_appenders.begin(),m_vct_appenders.end());
            }
            vct_appenders.swap(m_vct_appenders);
        }
    }

}