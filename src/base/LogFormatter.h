/**
 * @copyright Copyright (c) 2022..
 * 
 * @file    LogFormatter.h
 * @brief   日志的格式化方式
 * @author  zhangafei
 * @date    2022-10-06
 * @email   1763029328@qq.com
 */
#ifndef LOG_FORMATTER_H
#define LOG_FORMATTER_H

#include <memory>
#include <vector>
#include "LogLevel.h"
#include "LogEvent.h"
#include "LogFormatterItem.h"
namespace afa
{
    class LogFormatter
    {
    public:
        typedef std::shared_ptr<LogFormatter> Ptr;
        typedef std::shared_ptr<LogFormatterItem> ItemPtr;
        /**
         * @brief   构造函数
         * @param   pattern     格式字符串
         * @author  zhangafei
         * @date    2022-10-06
         */
        LogFormatter(const std::string& pattern = "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%p%T%c%T%f%M%l%T%m%n");
        
        /**
         * @brief   初始化
         * @author  zhangafei
         * @date    2022-10-06
         */
        void Init();
        /**
         * @brief   格式化输出
         * @param   os          输出流
         * @param   event       日志事件
         * @author  zhangafei
         * @date    2022-10-06
         */
        void Format(std::ostream &os,LogEvent::Ptr event);

        /**
         * @brief   设置日志格式
         * @param   newPattern  参数描述
         * @author  zhangafei
         * @date    2022-10-06
         */
        void SetFormat(const std::string &newPattern);

        /**
         * @brief   获取查看日志格式
         * @return  std::string 
         * @author  zhangafei
         * @date    2022-10-06
         */
        std::string GetFormat()
        {
            return m_pattern;
        }

    private:
        std::string          m_pattern;   //日志格式模板
        std::vector<ItemPtr> m_vctItems;  //日志输出项
    };
}
#endif