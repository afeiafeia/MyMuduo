#ifndef LOG_LEVEL_H
#define LOG_LEVEL_H
#include <string>

namespace afa
{
    class LogLevel
    {
    public:
        enum Level
        {
            UNKNOWN = 0,
            DEBUG   = 1,
            INFO    = 2,
            WARN    = 3,
            ERROR   = 4,
            FATAL   = 5   
        };

        /**
         * @brief  输出字符串格式的日志等级
         * @param  level            
         * @author afei
         * @date   2022-03-25
         * @return std::string 
         */
        static std::string To_String(LogLevel::Level level);
        
        /**
         * @brief  将字符串解析回日志等级
         * @param  strLevel         
         * @author afei
         * @date   2022-03-25
         * @return Level 
         */
        static LogLevel::Level To_Level(const std::string &strLevel);
    };
}
#endif