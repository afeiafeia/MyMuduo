#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H
//
#include "Logger.h"
#include "MutexLock.h"
#include "MutexLockGuard.h"
//C++
#include <map>
namespace afa
{
    const std::string RootLogger = "Root";   
    class LogManager
    {
    private:
        Logger::Ptr m_rootLogger;
        std::map<std::string,Logger::Ptr> m_mapLogger;
        MutexLock m_lock;
    private:
        LogManager()
        :m_rootLogger(){}
        ~LogManager(){};
        
    public:
        static LogManager* GetInstance()
        {
            static LogManager single_ton;
            return &single_ton;
        }

        Logger::Ptr GetRoot()
        {
            return GetLog("root");
        }

        Logger::Ptr GetLog(const std::string &s)
        {
            MutexLockGuard guard(&m_lock);
            if(m_mapLogger.find(s)==m_mapLogger.end())
            {
                m_mapLogger[s] = Logger::Ptr(new Logger(s));
            }
            return m_mapLogger[s];
        }


    };

}

#endif