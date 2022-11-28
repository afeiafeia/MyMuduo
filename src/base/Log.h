/**
 * @copyright Copyright (c) 2022..
 * 
 * @file    Log.h
 * @brief   日志
 * @author  zhangafei
 * @date    2022-09-15
 * @email   1763029328@qq.com
 */
#ifndef Log_H
#define Log_H

#include "CurrentThread.h"
#include "LogEventWarp.h"
#include "LogManager.h"
#include "LogLevel.h"
#define LOG_ROOT() afa::LogManager::GetInstance()->GetRoot()

#define LOG_NAME(name) afa::LogManager::GetInstance()->GetLog(name)

//流式输出日志
#define LOG_LEVEL(logger,level) \
    if(logger->GetLevel()<=level) \
         afa::LogEventWarp(logger,afa::LogEvent::Ptr(new afa::LogEvent(level, \
        __FILE__,__func__,__LINE__,afa::CurrentThread::tid(),\
        0,time(0),std::string(afa::CurrentThread::name())))).GetSS()

#define LOG_DEBUG(logger) LOG_LEVEL(logger,afa::LogLevel::DEBUG)

#define LOG_INFO(logger) LOG_LEVEL(logger,afa::LogLevel::INFO)

#define LOG_WARN(logger) LOG_LEVEL(logger,afa::LogLevel::WARN)

#define LOG_ERROR(logger) LOG_LEVEL(logger,afa::LogLevel::ERROR)

#define LOG_FATAL(logger) LOG_LEVEL(logger,afa::LogLevel::FATAL)


//格式化输出日式
#define LOG_FMT_LEVEL(logger,level,fmt,...) \
    if(logger->GetLevel()<=level) \
        afa::LogEventWarp(logger,afa::LogEvent::Ptr(new afa::LogEvent(level, \
        __FILE__,__func__,__LINE__,afa::CurrentThread::tid(), \
        0,time(0),std::string(afa::CurrentThread::name())) \
        )).format(fmt,__VA_ARGS__)



//Debug级别
#define LOG_FMT_DEBUG(logger,fmt,...)  LOG_FMT_LEVEL(logger,afa::LogLevel::DEBUG,fmt, __VA_ARGS__)
//Info级别
#define LOG_FMT_INFO(logger,fmt,...)   LOG_FMT_LEVEL(logger,afa::LogLevel::INFO,fmt, __VA_ARGS__)
//Warn级别
#define LOG_FMT_WARN(logger,fmt,...)   LOG_FMT_LEVEL(logger,afa::LogLevel::WARN,fmt, __VA_ARGS__)
//Error级别
#define LOG_FMT_ERROR(logger,fmt,...)  LOG_FMT_LEVEL(logger,afa::LogLevel::ERROR,fmt, __VA_ARGS__)
//Fatal级别
#define LOG_FMT_FATAL(logger,fmt,...)  LOG_FMT_LEVEL(logger,afa::LogLevel::FATAL,fmt, __VA_ARGS__)




#endif
    