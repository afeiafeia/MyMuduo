#include "LogFormatter.h"
#include <map>
#include <functional>

#include <iostream>
namespace afa
{
    LogFormatter::LogFormatter(const std::string &pattern)
    :m_pattern(pattern)
    {
        Init();
    }

    void LogFormatter::Init()
    {
        int n = m_pattern.size();
        std::vector<std::pair<std::string,std::string>> vct_items;
        for(int i=0;i<n;i++)
        {
            std::string pre_str;
            if(m_pattern[i]!='%')
            {
                continue;
            }

            int index = i+1;//从一个%的下一个字符开始，i处为%
            std::string cur_str;
            std::string format_str;
            int date_begin = i+1;
            int status = 0;
            int placeHolders = 0;
            while(index<n)
            {
                if(m_pattern[index]>='0'&&m_pattern[index]<='9')
                {
                    //是数字
                    while(m_pattern[index]!='%'&&index<n){
                        placeHolders = placeHolders*10+(m_pattern[index]-'0');
                        index++;
                    }
                    i = index++;
                    //std::cout<<placeHolders<<std::endl;
                    continue;
                }
                if(!status&&!isalpha(m_pattern[index])&&m_pattern[index]!='{'&&m_pattern[index]!='}')
                {
                    //正常格式下，在index处为%时进入
                    //curStr为%前的字符
                    cur_str = m_pattern.substr(i+1,index-i-1);
                    break;
                }

                if(!status&&m_pattern[index]=='{')
                {
                    cur_str = m_pattern.substr(i+1,index-i-1);
                    status = 1;
                    date_begin = index+1;
                }
                else if(status&&m_pattern[index]=='}')
                {
                    status = 0;
                    format_str = m_pattern.substr(date_begin,index-date_begin);
                    ++index;
                    break;
                }
                ++index;
                if(index==n)
                {
                    if(cur_str.empty())
                    {
                        cur_str = m_pattern.substr(i+1);
                        vct_items.push_back({cur_str,format_str});
                    }
                }
            }
            if(!cur_str.empty()&&vct_items.empty())
            {
                vct_items.push_back({cur_str,format_str});
            }
            i = index-1;//后续++会使得i从%开始
            static std::map<std::string,std::function<LogFormatterItem::Ptr(const std::string &s,int place)>> s_map_format_item = {
#define XX(str,C) \
            {#str,[](const std::string &s,int place){return LogFormatterItem::Ptr(new C(s,place));}}

            XX(m,ContentFormatterItem),   //消息
            XX(p,LevelFormatterItem),     //日志等级
            XX(t,ThreadIdFormatterItem),  //线程Id
            XX(n,NewLineFormatterItem),   //换行
            XX(d,TimeFormatterItem),      //时间
            XX(l,LineFormatterItem),      //行号
            XX(M,MethodFormatterItem),    //消息
            XX(F,FiberIdFormatterItem),   //协程Id
            XX(N,ThreadNameFormatterItem),//线程名称
            XX(f,FileFormatterItem),      //文件名称
            XX(T,TabFormatterItem),       //制表符
#undef XX
            };

            int num_item = vct_items.size();
            for(int j=0;j<num_item;++j)
            {
                std::string name = vct_items[j].first;
                std::string content = vct_items[j].second; 
                if(s_map_format_item.find(name)!=s_map_format_item.end())
                {
                    //std::cout<<placeHolders<<std::endl;
                    m_vctItems.push_back(s_map_format_item[name](content,placeHolders));
                }
            }
            vct_items.clear();
        }
       
    }

    void LogFormatter::Format(std::ostream &os,LogEvent::Ptr event)
    {
        for(auto& it:m_vctItems)
        {
            it->Format(os,event);
        }
    }

    void LogFormatter::SetFormat(const std::string &newPattern)
    {
        m_pattern = newPattern;
        Init();
    }
}