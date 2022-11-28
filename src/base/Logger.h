/**
 * @copyright Copyright (c) 2022..
 * 
 * @file    Logger.h
 * @brief   日志器
 * @author  zhangafei
 * @date    2022-09-15
 * @email   1763029328@qq.com
 */
#ifndef LOGGER_H
#define LOGGER_H

#include "noncopyable.h"
#include <string>
#include <list>
#include "LogLevel.h"
#include "LogAppender.h"
#include "LogEvent.h"
#include "MutexLock.h"
#include "MutexLockGuard.h"
#include <memory>
namespace afa
{

    class LogAppender;
    class LogEvent;
    class Logger: public std::enable_shared_from_this<Logger>
    {
    public:
        typedef std::shared_ptr<Logger>  Ptr;
        typedef LogLevel::Level level;
    private:
        std::string                   m_name;         //日志器名称
        LogLevel::Level               m_level;        //日志级别
        std::list<LogAppender::Ptr>   m_vctAppenders; //日志输出器
        MutexLock                     m_lock;         //互斥锁

    public:
        Logger(const std::string &name,level logLevel=LogLevel::DEBUG);
        ~Logger();

        void AddAppender(LogAppender::Ptr addAppender);

        void Log(LogLevel::Level level,LogEvent::Ptr event);
 

        LogLevel::Level GetLevel()
        {
            return m_level;
        }


        bool IsRoot()
        {
            return m_name=="root";
        }


    };
}
#endif
