#ifndef LOG_FILE_APPENDER_H
#define LOG_FILE_APPENDER_H
#include <iostream>
#include <fstream>
#include <memory>
#include "Logger.h"
#include "LogEvent.h"
#include "LogLevel.h"
#include "LogAppender.h"
namespace afa
{
    class LogFileAppender:public LogAppender
    {
    public:
        typedef std::shared_ptr<LogFileAppender> Ptr;
    private:
        std::ofstream m_file_stream;//文件流
        std::string   m_file_path;//文件路径
    public:

        LogFileAppender(const std::string &path);

        void Log(LogLevel::Level level,LogEvent::Ptr event);
    private:
        void OpenFile();
    };
}


#endif