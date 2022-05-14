#ifndef HTTP_DATA_H
#define HTTP_DATA_H

#define READ_BUFF_SIZE 1024
#define WRITE_BUFF_SIZE 1024
#include <memory>
#include <map>
#include <netinet/in.h>
#include <sys/stat.h>
#include <bits/types/struct_iovec.h>
#include "../Buffer.h"
namespace afa
{
    class Channel;
    class HttpServer;
    typedef std::shared_ptr<Channel> SP_Channel;
    class Http_Data:public std::enable_shared_from_this<Http_Data>
    {
    public:
        enum Method
        {
            GET=0,
            POST,
            HEAD,
            PUT,
            DELETE,
            TRACE,
            OPTIONS,
            CONNECT,
            PATH
        };
    
        enum CHECK_STATE
        {
            CHECK_STATE_REQUESTLINE=0,
            CHECK_STATE_HEADER,
            CHECK_STATE_CONTNET
        };
        enum LINE_STATE
        {
            LINE_OK=0,
            LINE_BAD,
            LINE_OPEN
        };
    
        enum HTTP_STATE
        {
            NO_REQUEST=0,//请求不完整，需要继续读取请求数据(跳转到主线程继续监测读事件)
            GET_REQUEST, //获得了完整的http请求
            BAD_REQUEST, //请求报文有语法错误
            FORBIDDEN_REQUEST, //服务器内部错误，该结果在主状态机逻辑switch的default下，一般不会触发
            INTERNAL_ERROR,
            CLOSED_CONNECTION,
            FILE_REQUEST
        };

        Http_Data(const Buffer &request);
        ~Http_Data()
        {

        }
        void AppendData(const Buffer &request);

        bool Parse();

        Buffer GetResponse()
        {
            return m_write_buff;
        }
    private:
        void Init();
        
        HTTP_STATE ParseRequest();

        //解析一行
        //确定当前行是否完整，如果完整，将末尾的\r\n替换为\0\0;并更新m_check_idx为下一行的第一个字符的在报文中的下标
        LINE_STATE ParseLine();
        //解析请求头
        HTTP_STATE ParseRequestLine(char* ipCurLine);

        //解析首部行
        HTTP_STATE ParseHeaders(char* ipCurLine);
        char* GetCurLine();

        //解析实体体
        HTTP_STATE ParseContent(char* ipCurLine);

        //修改监测读事件
        void modfd(int event);

        //会根据解析出来的请求，打开相应的文件并映射到虚拟地址空间
        HTTP_STATE DoRequest();

        //这一步并不是发送报文，而是将报文存储信息iovec中，
        //之后，process中会注册监测套接字的可写事件，
        //可写时，工作线程调用write进行响应，将报文发送给浏览器
        bool ConstructResponse(HTTP_STATE ret);

        bool AddResponse(const char* format,...);

        //添加状态行
        bool AddStatusLine(int status,const char* title);
        //添加消息报头
        bool AddHeaders(int connext_status);
        //向首部行添加conteng_len字段，表示响应报文实体体的长度
        bool AddContentLength(int content_len);
        //添加文本类型content_Type字段
        bool AddContentType();
        //添加连接状态connection字段：close(短连接)或者keep-alive（长连接）
        bool AddConnection();
        //添加空行
        bool AddBlankLine();
        //添加实体体
        bool AddContent(const char* content);

        bool Write();

        void Swap(Buffer& buffer)
        {
            m_read_buff.Swap(buffer);
        }

    private:
        CHECK_STATE m_check_state;

        Buffer m_read_buff;
        Buffer m_write_buff;

        //char m_read_buff[READ_BUFF_SIZE]; //读缓冲区（套接字可读时，将数据读到此缓冲区）
        //int m_read_index;                 //读取到的字节数，而不是最后一个字节的下标
//
        //char m_write_buff[WRITE_BUFF_SIZE];
        //int  m_write_index;
        //这里的读缓冲区存在一个问题：缓冲区大小是固定的，而且比较小，如果一个报文比较大，缓冲区放不下，读取的报文就不完整
        //muduo中使用了临时栈上空间，使用readv读取，详看muduo7.4.7节

        //写的时候使用了struct iovec，不存在此问题
        int m_start_line;//每次解析一行之后，m_start_line指向该行的第一个字符在m_read_buff中的下标，而m_check_index指向下一行待解析的行首下标
        int m_check_index;
        int m_length;

        //请求行
        char* m_url;//URL
        int   m_cgi;  //对应post请求，post请求下，值为1（表示true），其他情况下为0（表示false）
        Method      m_method;//方法
        char*       m_version;//版本

        //首部行
        linger m_LingerOpt;
        bool   m_linger;//长、短连接，true表示长连接
        int    m_ContentLength; //实体体长度
        char*  m_browser;//浏览器版本
        char*  m_host;//主机域名
        char*  m_file_address;

        //实体体
        std::string m_content;  //用户名+密码

        
        //登录信息
        std::map<std::string,std::string> m_users;

        bool m_parse_done;
        //
        char* m_root;//根目录
        char  m_real_file_path[256];  //所请求文件所在的路径
        struct stat m_file_stat;//文件的属性信息
    };
}
#endif