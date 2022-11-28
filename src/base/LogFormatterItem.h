#ifndef LOG_FORMATTER_ITEM_H
#define LOG_FORMATTER_ITEM_H

#include <memory>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
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
    protected:
        void output(std::ostream &os,std::string content,int placeHolders)
        {
            int charCount = static_cast<int>(content.size());
            if(charCount<placeHolders)
            {
                os<<"["<<content;
                int count = placeHolders - charCount;
                while(count--)
                {
                    os<<" ";
                }
                os<<']';
            }
            else
            {
                os<<'['<<content.substr(0,placeHolders)<<']';
            }
        }

    };

    /**
     * @brief  日志等級
     * @author afei
     * @date   2022-03-26
     */
    class LevelFormatterItem:public LogFormatterItem
    {
    public:
        LevelFormatterItem(const std::string &s = "",int placeCount=0):placeHolders(placeCount){}

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            std::string level = LogLevel::To_String(event->GetLogLevel());
            if (placeHolders==0)
            {
                os<<"["<<level<<"]";
            }
            else
            {
                output(os,level,placeHolders);
            }
        }
    private: 
        int placeHolders;
    };

    /**
     * @brief  输出日志语句的文件的文件名
     * @author afei
     * @date   2022-03-26
     */
    class FileFormatterItem:public LogFormatterItem
    {
    public:
        FileFormatterItem(const std::string &s = "",int placeCount=0):placeHolders(placeCount){}

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            os<<"["<<event->GetFileName()<<":";
        }
    private: 
        int placeHolders;
    };

    class MethodFormatterItem:public LogFormatterItem
    {
    public:
        MethodFormatterItem(const std::string &s = "",int placeCount=0):placeHolders(placeCount){}

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            os<<event->GetMethodName()<<"()"<<":";
        }
    private: 
        int placeHolders;
    };

    /**
     * @brief  行号
     * @author afei
     * @date   2022-03-26
     */
    class LineFormatterItem:public LogFormatterItem
    {
    public:
        LineFormatterItem(const std::string &s = "",int placeCount=0):placeHolders(placeCount){}

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            os<<event->GetLine()<<"]";
        }
    private: 
        int placeHolders;
    };

        /**
     * @brief  换行
     * @author afei
     * @date   2022-03-26
     */
    class NewLineFormatterItem:public LogFormatterItem
    {
    public:
        NewLineFormatterItem(const std::string &s = "",int placeCount=0):placeHolders(placeCount){}

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            os<<"\n";
        }
    private: 
        int placeHolders;
    };

    /**
     * @brief  线程Id
     * @author afei
     * @date   2022-03-26
     */
    class ThreadIdFormatterItem:public LogFormatterItem
    {
    public:
        ThreadIdFormatterItem(const std::string &s = "",int placeCount=0):placeHolders(placeCount){}

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            if(placeHolders==0)
            {
                os<<"["<<event->GetThreadId()<<"]";
            }
            else
            {
                char buff[16]={'\0'};
                sprintf(buff,"%d",static_cast<int>(event->GetThreadId()));
                output(os,std::string(buff),placeHolders);
            }
        }
    private: 
        int placeHolders;
    };

    /**
     * @brief  协程Id
     * @author afei
     * @date   2022-03-26
     */
    class FiberIdFormatterItem:public LogFormatterItem
    {
    public:
        FiberIdFormatterItem(const std::string &s = "",int placeCount=0):placeHolders(placeCount){}

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            os<<"["<<event->GetFiberId()<<"]";
        }
    private: 
        int placeHolders;
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
        TimeFormatterItem(const std::string &s = "%Y-%m-%d %H:%M:%S",int placeCount=0)
        :m_pattern(s)
        ,placeHolders(placeCount)
        {

        }

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            struct tm t;
            time_t time = event->GetTime();
            localtime_r(&time,&t);
            char buff[64]={'\0'};
            strftime(buff,sizeof(buff),m_pattern.c_str(),&t);
            if(placeHolders==0)
            {
                os<<"["<<buff<<"]";
            }
            else
            {
                output(os,std::string(buff),placeHolders);
            }
        }
    private: 
        int placeHolders;
    };

    /**
     * @brief  线程名称
     * @author afei
     * @date   2022-03-26
     */
    class ThreadNameFormatterItem:public LogFormatterItem
    {
    public:
        ThreadNameFormatterItem(const std::string &s = "",int placeCount=0):placeHolders(placeCount){}

        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            if(placeHolders==0)
            {
                os<<"["<<event->GetThreadName()<<"]";
            }
            else
            {
                output(os,event->GetThreadName(),placeHolders);
            }
        }
    private: 
        int placeHolders;
    };

    /**
     * @brief  日志内容
     * @author afei
     * @date   2022-03-26
     */
    class ContentFormatterItem:public LogFormatterItem
    {
    public:
        ContentFormatterItem(const std::string &s="",int placeCount=0):placeHolders(placeCount){}
        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            if(placeHolders==0)
            {
                os<<"["<<event->GetContent()<<"]";
            }
            else
            {
                output(os,event->GetContent(),placeHolders);
            }
        }
    private: 
        int placeHolders;

    };

    /**
     * @brief  制表符
     * @author afei
     * @date   2022-04-16
     */
    class TabFormatterItem:public LogFormatterItem
    {
    public:
        TabFormatterItem(const std::string &s="",int placeCount=0):placeHolders(placeCount){}
        void Format(std::ostream &os,LogEvent::Ptr event)
        {
            os<<"  ";//制表符
        }
    private: 
        int placeHolders;

    };




}
#endif