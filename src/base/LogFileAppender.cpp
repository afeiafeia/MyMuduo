#include "LogFileAppender.h"
namespace afa
{
    LogFileAppender::LogFileAppender(const std::string &path)
    :m_file_path(path)
    {
        OpenFile();
    }

    void LogFileAppender::Log(LogLevel::Level level,LogEvent::Ptr event)
    {
        if(level<GetLevel())
        {
            return ;
        }
        GetFormatter()->Format(m_file_stream,event);
        m_file_stream.flush();
    }

    void LogFileAppender::OpenFile()
    {
        if(m_file_stream)
        {
            m_file_stream.close();
        }
        m_file_stream.open(m_file_path.c_str(), std::ios::app);   

    }
}