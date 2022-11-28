/**
 * @copyright Copyright (c) 2022..
 * 
 * @file    LogFileAppender.h
 * @brief   日志的文件输出地
 * @author  zhangafei
 * @date    2022-10-06
 * @email   1763029328@qq.com
 */
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
        std::ofstream m_fileStream;//文件流
        std::string   m_filePath;//文件路径
        MutexLock      m_lock;         //互斥锁
    public:
        /**
         * @brief   构造函数
         * @param   path        文件路径
         * @author  zhangafei
         * @date    2022-10-06
         */
        LogFileAppender(const std::string &path);
        
        /**
         * @brief   析构函数
         * @author  zhangafei
         * @date    2022-10-06
         */
        ~LogFileAppender();

        /**
         * @brief   日志输出
         * @param   level       日志等级
         * @param   event       日志事件
         * @author  zhangafei
         * @date    2022-10-06
         */
        void Log(LogLevel::Level level,LogEvent::Ptr event);
    private:
        /**
         * @brief   以流的方式打开文件
         * @author  zhangafei
         * @date    2022-10-06
         */
        void OpenFile();
    };
}


#endif