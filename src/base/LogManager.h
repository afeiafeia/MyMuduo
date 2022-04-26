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
    class LogManager
    {
    private:
        Logger::Ptr m_root_logger;
        std::map<std::string,Logger::Ptr> m_map_logger;
        MutexLock m_lock;
    private:
        LogManager()
        :m_root_logger(new Logger("root"))
        {
            m_map_logger["root"] = m_root_logger;
        }
        ~LogManager(){};
        
    public:
        static LogManager* GetInstance()
        {
            static LogManager single_ton;
            return &single_ton;
        }

        Logger::Ptr GetRoot()
        {
            return m_root_logger;
        }

        Logger::Ptr GetLog(const std::string &s)
        {
            MutexLockGuard guard(&m_lock);
            if(m_map_logger.find(s)==m_map_logger.end())
            {
                m_map_logger[s] = Logger::Ptr(new Logger(s));
            }
            return m_map_logger[s];
        }


    };

}

#endif