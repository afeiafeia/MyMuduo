#include "LogFileAppender.h"
namespace afa
{
    LogFileAppender::LogFileAppender(const std::string &path)
    :m_filePath(path)
    ,m_lock()
    {
        OpenFile();
    }

    LogFileAppender::~LogFileAppender()
    {
        m_fileStream.flush();
        m_fileStream.close();
    }

    void LogFileAppender::Log(LogLevel::Level level,LogEvent::Ptr event)
    {
        
        MutexLockGuard guard(&m_lock);
        if(level<GetLevel())
        {
            return ;
        }
        GetFormatter()->Format(m_fileStream,event);
        m_fileStream.flush();
        

    }

    void LogFileAppender::OpenFile()
    {
        if(m_fileStream)
        {
            m_fileStream.close();
        }
        m_fileStream.open(m_filePath.c_str(), std::ios::app);   

    }
}