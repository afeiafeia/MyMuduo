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
        //日志名称改用：名称+日期
        //获取当前日期
        time_t cur = time(NULL);
        struct tm* sys_time = localtime(&cur);
        char logFileName[128] = {'\0'};
        if(!IsRoot())
        {
            snprintf(logFileName,128,"%s_%d_%02d_%02d",m_name.c_str(),sys_time->tm_year+1900,sys_time->tm_mon+1,sys_time->tm_mday);
        }
        else
        {
            //root日志器的日志名称，直接使用日期
            snprintf(logFileName,128,"%d_%02d_%02d",sys_time->tm_year+1900,sys_time->tm_mon+1,sys_time->tm_mday);
        }

        //日志写入当前路径

        LogAppender::Ptr fileAppender(new LogFileAppender("./"+std::string(logFileName)+".log"));
        m_vctAppenders.push_back(fileAppender);
    }
    
    Logger::~Logger()
    {

    }



    void Logger::AddAppender(LogAppender::Ptr appender)
    {
        MutexLockGuard guard(&m_lock);
        m_vctAppenders.push_back(appender);
    }



    void Logger::Log(LogLevel::Level level,LogEvent::Ptr event)
    {
        if(level<m_level)
        {
            return;
        }

        //防止在一个线程写日志时，另外的线程正在AddAppender
        std::list<LogAppender::Ptr> vctAppenders;
        {
            MutexLockGuard guard(&m_lock);
            vctAppenders.swap(m_vctAppenders);
        }
        for(auto& it:vctAppenders)
        {
            it->Log(level,event);
        }
        {
            MutexLockGuard guard(&m_lock);
            if(!m_vctAppenders.empty())
            {
                vctAppenders.insert(vctAppenders.end(),m_vctAppenders.begin(),m_vctAppenders.end());
            }
            vctAppenders.swap(m_vctAppenders);
        }
    }

}