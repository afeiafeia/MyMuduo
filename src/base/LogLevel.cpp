#include "LogLevel.h"
#include <strings.h>
namespace afa
{
    std::string LogLevel::To_String(LogLevel::Level level)
    {
        switch (level)
        {
        #define XX(name) \
        case LogLevel::name: \
             return #name;

             XX(DEBUG);
             XX(INFO);
             XX(WARN);
             XX(ERROR);
             XX(FATAL);
        #undef XX
        
        default:
            return "UNKNOWN";
        }
        return "UNKNOWN";
    }

    LogLevel::Level LogLevel::To_Level(const std::string &level)
    {
        if((strcasecmp(level.c_str(),"debug"))==0)
        {
            return LogLevel::DEBUG;
        }
        else if((strcasecmp(level.c_str(),"info"))==0)
        {
            return LogLevel::INFO;
        }
        else if((strcasecmp(level.c_str(),"warn"))==0)
        {
            return LogLevel::WARN;
        }
        else if((strcasecmp(level.c_str(),"error"))==0)
        {
            return LogLevel::ERROR;
        }
        else if((strcasecmp(level.c_str(),"fatal"))==0)
        {
            return LogLevel::FATAL;
        }
        else
        {
            return LogLevel::UNKNOWN;
        }

    }



}
