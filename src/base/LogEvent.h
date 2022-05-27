#ifndef LOG_EVENT_H
#define LOG_EVENT_H
#include <sstream>
#include <string>
#include <memory>
#include "LogLevel.h"
namespace afa
{
    class Logger;
    class LogEvent
    {
    public:
        typedef std::shared_ptr<LogEvent> Ptr;
    private:
        LogLevel::Level          m_level;           //日志级别
        std::string              m_file_name;       //日志语句所在文件，通过__FILE__宏得到
        std::string              m_method_name;     //日志语句所在函数，通过__func_宏得到
        u_int32_t                m_line;            //行号
        u_int32_t                m_thread_id;       //线程id
        u_int32_t                m_fiber_id;        //协程id
        u_int64_t                m_time;            //时间
        std::string              m_thread_name;     //线程名称
        std::string              m_format_content;  //格式化字符串，用于格式化输出日志时存储日志内容
        std::stringstream        m_content_stream;  //字符串流，用于流式输出日志时存储日志内容

    public:
        LogEvent(LogLevel::Level level,
                std::string      fileName,
                std::string      methodName,
                u_int32_t        line,
                u_int32_t        threadId,
                u_int32_t        fiberId,
                u_int64_t        time,
                std::string      threadName);

        ~LogEvent();


        /**
         * @brief  获取日志级别
         * @author afei
         * @date   2022-03-25
         * @return LogLevel::Level 
         */
        LogLevel::Level GetLogLevel(){return m_level;}

        /**
         * @brief  获取创建日志的文件的文件名称
         * @author afei
         * @date   2022-03-25
         * @return std::string 
         */
        std::string GetFileName(){return m_file_name;}


        std::string GetMethodName(){return m_method_name;}


        /**
         * @brief  获取行号
         * @author afei
         * @date   2022-03-25
         * @return u_int32_t 
         */
        u_int32_t GetLine(){return m_line;}

        /**
         * @brief  获取日志的创建时间
         * @author afei
         * @date   2022-03-25
         * @return u_int64_t 
         */
        u_int64_t GetTime(){return m_time;}

        /**
         * @brief  获取创建日志的线程的id
         * @author afei
         * @date   2022-03-25
         * @return u_int32_t 
         */
        u_int32_t GetThreadId(){return m_thread_id;}

        /**
         * @brief  获取创建日志的协程的id
         * @author afei
         * @date   2022-03-25
         * @return u_int32_t 
         */
        u_int32_t GetFiberId(){return m_fiber_id;}

        /**
         * @brief  获取创建日志的线程的名称
         * @author afei
         * @date   2022-03-25
         * @return std::string 
         */
        std::string GetThreadName(){return m_thread_name;}

        /**
         * @brief  获取日志内容
         * @author afei
         * @date   2022-03-25
         * @return std::string 
         */
        std::string GetContent(){return m_content_stream.str();}

        std::stringstream& GetSS(){return m_content_stream;}

        /**
         * @brief  格式化写入日志内容
         * @author afei
         * @date   2022-03-25
         */
        void Format(const char* formatContent,...);

        /**
         * @brief  格式化写入日志内容
         * @param  formatContent    
         * @param  va               
         * @author afei
         * @date   2022-03-25
         */
        void Format(const char* formatContent,va_list va);

    };
}
#endif