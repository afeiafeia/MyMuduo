#ifndef LOG_FORMATTER_ITEM_H
#define LOG_FORMATTER_ITEM_H

#include <memory>
#include <iostream>
#include "LogEvent.h"
#include "LogLevel.h"
namespace afa
{
    class LogFormatterItem
    {
    public:
        typedef std::shared_ptr<LogFormatterItem> Ptr;

        virtual ~LogFormatterItem(){}

        virtual void Format(std::ostream &os,LogEvent::Ptr event)=0;

    };

    /**
     * @brief  日志等級
     * @author afei
     * @date   2022-03-26
     */
    class LevelFormatterItem:public LogFormatterItem
    {
    public:
        LevelFormatterItem(const std::string &s = ""){}

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            //std::string ss = LogLevel::To_String(level);
            os<<"["<<LogLevel::To_String(event->GetLogLevel())<<"]";
        }
    };

    /**
     * @brief  输出日志语句的文件的文件名
     * @author afei
     * @date   2022-03-26
     */
    class FileFormatterItem:public LogFormatterItem
    {
    public:
        FileFormatterItem(const std::string &s = ""){}

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            os<<"["<<event->GetFileName()<<":";
        }
    };

    class MethodFormatterItem:public LogFormatterItem
    {
    public:
        MethodFormatterItem(const std::string &s = ""){}

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            os<<event->GetMethodName()<<"()"<<":";
        }
    };

    /**
     * @brief  行号
     * @author afei
     * @date   2022-03-26
     */
    class LineFormatterItem:public LogFormatterItem
    {
    public:
        LineFormatterItem(const std::string &s = ""){}

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            os<<event->GetLine()<<"]";
        }
    };

        /**
     * @brief  换行
     * @author afei
     * @date   2022-03-26
     */
    class NewLineFormatterItem:public LogFormatterItem
    {
    public:
        NewLineFormatterItem(const std::string &s = ""){}

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            os<<"\n";
        }
    };

    /**
     * @brief  线程Id
     * @author afei
     * @date   2022-03-26
     */
    class ThreadIdFormatterItem:public LogFormatterItem
    {
    public:
        ThreadIdFormatterItem(const std::string &s = ""){}

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            os<<"["<<event->GetThreadId()<<"]";
        }
    };

    /**
     * @brief  协程Id
     * @author afei
     * @date   2022-03-26
     */
    class FiberIdFormatterItem:public LogFormatterItem
    {
    public:
        FiberIdFormatterItem(const std::string &s = ""){}

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            os<<"["<<event->GetFiberId()<<"]";
        }
    };
    
    /**
     * @brief  时间
     * @author afei
     * @date   2022-03-26
     */
    class TimeFormatterItem:public LogFormatterItem
    {
    private:
        std::string m_pattern;
    public:
        TimeFormatterItem(const std::string &s = "%Y-%m-%d %H:%M:%S")
        :m_pattern(s)
        {

        }

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            struct tm t;
            time_t time = event->GetTime();
            localtime_r(&time,&t);
            char buff[64];
            strftime(buff,sizeof(buff),m_pattern.c_str(),&t);
            os<<"["<<buff<<"]";
        }
    
    };

    /**
     * @brief  线程名称
     * @author afei
     * @date   2022-03-26
     */
    class ThreadNameFormatterItem:public LogFormatterItem
    {
    public:
        ThreadNameFormatterItem(const std::string &s = ""){}

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            os<<"["<<event->GetThreadName()<<"]";
        }
    };

    /**
     * @brief  日志内容
     * @author afei
     * @date   2022-03-26
     */
    class ContentFormatterItem:public LogFormatterItem
    {
    public:
        ContentFormatterItem(const std::string &s=""){}
        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            os<<"["<<event->GetContent()<<"]";
        }

    };

    /**
     * @brief  制表符
     * @author afei
     * @date   2022-04-16
     */
    class TabFormatterItem:public LogFormatterItem
    {
    public:
        TabFormatterItem(const std::string &s=""){}
        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            os<<'\t';//制表符
        }

    };




}
#endif