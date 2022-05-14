#include "HttpData.h"
#include "../Channel.h"
#include "../../base/Log.h"
#include <string.h>
#include <sys/unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdarg.h>
namespace afa
{
    const char *ok_200_title = "OK";
    const char *error_400_title = "Bad Request";
    const char *error_400_form = "Your request has bad syntax or is inherently impossible to staisfy.\n";
    const char *error_403_title = "Forbidden";
    const char *error_403_form = "You do not have permission to get file form this server.\n";
    const char *error_404_title = "Not Found";
    const char *error_404_form = "The requested file was not found on this server.\n";
    const char *error_500_title = "Internal Error";
    const char *error_500_form = "There was an unusual problem serving the request file.\n";

    Logger::Ptr http_logger = LOG_NAME("HttpServer");

    Http_Data::Http_Data(const Buffer &request)
    {
        m_read_buff.Append(request.Peek(),request.ReadableBytes());
        m_length = m_read_buff.ReadableBytes();
        Init();
    }

    void Http_Data::AppendData(const Buffer &request)
    {
        m_read_buff.Append(request.Peek(),request.ReadableBytes());
        m_length = m_read_buff.ReadableBytes();
    }

    void Http_Data::Init()
    {
        m_check_state = CHECK_STATE_REQUESTLINE;
        m_url = nullptr;
        m_version = nullptr;

        m_method = GET;
        m_ContentLength = 0;
        m_content = nullptr;
        m_check_index= 0;
        m_start_line = 0;
        m_linger = false;

        m_browser = nullptr;//浏览器版本
        m_host = nullptr;//主机域名
        m_file_address = nullptr;

        memset(m_real_file_path,'\0',256);
        m_parse_done = false;

    }

    bool Http_Data::Parse()
    {
        HTTP_STATE res = ParseRequest();
        if(res==HTTP_STATE::NO_REQUEST)
        {
            return false;
        }
        return ConstructResponse(res);
    }

    Http_Data::LINE_STATE Http_Data::ParseLine()
    {
        char tmp;
        for(;m_check_index<=m_length;m_check_index++)
        {
            tmp = m_read_buff[m_check_index];
            if(tmp=='\r')
            {
                //'\r'是最后一个字符，读取不完整，继续读取
                if(m_check_index+1==m_length)
                {
                    return Http_Data::LINE_OPEN;
                }
                if(m_read_buff[m_check_index+1]=='\n')
                {
                    m_read_buff[m_check_index++] = '\0';
                    m_read_buff[m_check_index++] = '\0';
                    return Http_Data::LINE_OK;
                }

            }
            //也可能没有\r，直接是\n,一般是上次接受到|\r就到末尾了，之后后续报文到达，再次读取时，直接读取\n
            else if(tmp=='\n')
            {
                //没有'\r'，只有一个'\n'
                if(m_check_index>0&&m_read_buff[m_check_index-1]=='\r')
                {
                    m_read_buff[m_check_index-1] = '\0';
                    m_read_buff[m_check_index++] = '\0';
                    return Http_Data::LINE_OK;
                }
            }
            return Http_Data::LINE_BAD;
        }
    }

    char* Http_Data::GetCurLine()
    {
        return m_read_buff.Peek()+m_start_line;
    }

    Http_Data::HTTP_STATE Http_Data::ParseRequestLine(char* ipCurLine)
    {
        //http报文中，请求头的各个字段之间以'\t'或者空格分割
        m_url = strpbrk(ipCurLine," \t");//如果m_url不为空，此时m_url执行报文中URL字段的前一个位置
        if(m_url==nullptr)
        {
            return Http_Data::BAD_REQUEST;
        }
        *m_url++ = '\0';   //m_url所指位置后移，指向实际的url，同时将之前所指的分割符置为'\0'
        char* pMethod = nullptr;
        if(strcasecmp(ipCurLine,"Get")==0)
        {
            m_method = GET;
        }
        else if(strcasecmp(ipCurLine,"Post")==0)
        {
            m_method = POST;
        }
        else 
        {
            return Http_Data::BAD_REQUEST;
        }

        //版本
        m_version = strpbrk(m_url," \t");
        if(m_version==nullptr)
        {
            return Http_Data::BAD_REQUEST;
        }
        if(strncasecmp(m_version,"HTTP/1.1",7)!=0)
        {
            return BAD_REQUEST;
        }
        if(strncasecmp(m_url,"http://",7)==0)
        {
            m_url+=7;
            //char *strchr(const char *str, int c);
            //在str所指向的字符串中检索字符c第一次出现的位置
            m_url = strchr(m_url,'/');
        }
        if(strncasecmp(m_url,"https://",8)==0)
        {
            //https协议
            m_url+=8;
            m_url = strchr(m_url,'/');
        }
        if(!m_url||m_url[0]!='/')
        {
            return BAD_REQUEST;
        }

        if(strlen(m_url)==1)
        {
            strcat(m_url,"judge.html");
        }
        m_check_state = CHECK_STATE_HEADER;
        return NO_REQUEST;
    }

    Http_Data::HTTP_STATE Http_Data::ParseRequest()
    {

        //初始化从状态机状态
        LINE_STATE line_state = LINE_OK;
        HTTP_STATE ret = NO_REQUEST;
        char* pCurLine= nullptr;

        while((m_check_state==CHECK_STATE_CONTNET&&line_state==LINE_OK)||((line_state=ParseLine())==LINE_OK))
        {
            //实体体不会被parse_line解析，当m_check_state==CHECK_STATE_CONTNET时，不会调用后面对parse_line，因此，m_check_index此时指向实体体的开头
            pCurLine = GetCurLine();
            m_start_line = m_check_index;

            switch(m_check_state)
            {
                case CHECK_STATE_REQUESTLINE:
                {
                    ret = ParseRequestLine(pCurLine);
                    if(ret==BAD_REQUEST)
                    {
                        return BAD_REQUEST;
                    }
                    break;
                }
                case CHECK_STATE_HEADER:
                {
                    ret = ParseHeaders(pCurLine);
                    if(ret==BAD_REQUEST)
                    {
                        return BAD_REQUEST;
                    }
                    else if(ret==GET_REQUEST)
                    {
                        //CHECK_STATE_HEADER状态下，只有在是Get请求时，才会返回Get_REQUEST
                        return DoRequest();
                    }
                    break;
                }
                case CHECK_STATE_CONTNET:
                {
                    ret = ParseContent(pCurLine);   
                    if(ret==GET_REQUEST)
                    {
                        return DoRequest();
                    }           
                    line_state = LINE_OPEN;
                    break;
                }
                default:
                return INTERNAL_ERROR;
            }
        }
        return NO_REQUEST;

    }
    Http_Data::HTTP_STATE Http_Data::ParseHeaders(char* ipCurLine)
    {
        if(*ipCurLine=='\0')
        {
            //前面的解析已将将每一行最后的\r\n替换为\0
            //当前解析的行是首部行最后一行（空行）
            if(m_ContentLength>0)
            {
                //说明是Post请求
                //切换状态
                m_check_state = CHECK_STATE_CONTNET;
                return NO_REQUEST;
            }
            return GET_REQUEST;
        }
        else if(strncasecmp(ipCurLine,"Connection:",11)==0)
        {
            ipCurLine+=11;
            ipCurLine += strspn(ipCurLine," \t");
            if(strncasecmp(ipCurLine,"keep-alive",10)==0)
            {
                m_linger = true;
            }
        }
        else if(strncasecmp(ipCurLine,"Content-Length:",15)==0)
        {
            ipCurLine+=15;
            ipCurLine+=strspn(ipCurLine," \t");
            m_ContentLength = atoi(ipCurLine);
        }
        else if(strncasecmp(ipCurLine,"Host:",5)==0)
        {
            ipCurLine+=15;
            ipCurLine+=strspn(ipCurLine," \t");
            m_host = ipCurLine;
        }
        return NO_REQUEST;
    }

    Http_Data::HTTP_STATE Http_Data::ParseContent(char* ipCurLine)
    {
        if(m_length>=m_ContentLength+m_check_index)
        {
            m_content = ipCurLine;
            m_content[m_ContentLength]='\0';
            return GET_REQUEST;
        }
        return NO_REQUEST;
    }

    Http_Data::HTTP_STATE Http_Data::DoRequest()
    {
        //将m_real_file_path设置为根目录
        strcpy(m_real_file_path,m_root);
 
        int len = strlen(m_real_file_path);
 
        //找到m_url中字符'/'的位置
        const char* p = strrchr(m_url,'/');//在m_url所指向的字符串中检索字符'/'最后一次出现的位置
        if(m_cgi==1&&(*(p+1)=='2'||*(p+1)=='3'))
        {
            //说明是m_cgi==1说明是post请求，*(p+1)=='2'或者*(p+1)=='3'说明是登录监测或者注册监测
            //对应的步骤是：注册账号密码之后点击提交或者输入账号密码之后点击确定，这里是通过'2'和'3'区分，这一步所解析的post请求的报文中，包含了账号和密码
            char url_real[256]={'\0'};
            strcpy(url_real,"/");
            strcat(url_real,m_url+2);
            strncpy(m_real_file_path+len,url_real,FILENAME_MAX-len-1);//-1是因为最后一位是预留给'/0';
 
            //提起用户名和密码:其在报文中的格式是user=xyz&passwd=abcd;//xyz、abcd表示任意数量任意字符
            char name[256] = {'\0'};
            char passwd[256] = {'\0'};
            char* userSt = strchr(const_cast<char*>(m_content.c_str()),'=');
            userSt++;
            char* passwdSt = strrchr(const_cast<char*>(m_content.c_str()),'=');
            passwdSt++;
            int i=0;
            for(;userSt[i]!='&';i++)
            {
                name[i] = userSt[i];
            }
            int j=0;
            for(;passwdSt[j]!='\0';j++)
            {
                passwd[j] = passwdSt[j];
            }
            if(*(p+1)=='3')
            {
                //是注册
                //如果是注册，先检测数据库中是否有重名的
                //没有重名的，进行增加数据
                char *sql_insert = (char *)malloc(sizeof(char) * 200);
                strcpy(sql_insert, "INSERT INTO user(username, passwd) VALUES(");
                strcat(sql_insert, "'");
                strcat(sql_insert, name);
                strcat(sql_insert, "', '");
                strcat(sql_insert, passwd);
                strcat(sql_insert, "')");
 
                if (m_users.find(name) == m_users.end())
                {
                    //int res = mysql_query(mysql, sql_insert);
                    m_users.insert(std::pair<std::string, std::string>(name, passwd));
                    strcpy(m_url, "/registerError.html");
                }
                else
                    strcpy(m_url, "/registerError.html");
 
            }
            //如果是登录，直接判断
            //若浏览器端输入的用户名和密码在表中可以查找到，返回1，否则返回0
            else if (*(p + 1) == '2')
            {
                if (m_users.find(name) != m_users.end() && m_users[name] == passwd)
                    strcpy(m_url, "/welcome.html");
                else
                    strcpy(m_url, "/logError.html");
            }
 
        }
 
        //为注册请求，跳转到注册页面register.html
        if(*(p+1)=='0')
        {
            const char* pResource = "/register.html";
            int length = strlen(pResource);
            strncpy(m_real_file_path+len,pResource,length);
        }
        //为登录请求，跳转到登录页面log.html
        else if(*(p+1)=='1')
        {
            const char* pResource = "/log.html";
            int length = strlen(pResource);
            strncpy(m_real_file_path+len,pResource,length);
        }
        else if(*(p+1)=='5')
        {
            //既不是注册也不是登录，是请求资源
            //这里的处理是响应欢迎界面
            //请求图片资源
            const char* pPicture = "/picture.html";
            int length = strlen(pPicture);
            strncpy(m_real_file_path+len,pPicture,length);
        }
        else if(*(p+1)=='6')
        {
            //请求视频资源
            const char* pVideo = "/video.html";
            int length = strlen(pVideo);
            strncpy(m_real_file_path+len,pVideo,length);
        }
 
        else if(*(p+1)=='7')
        {
            //请求fans资源
            const char* pFans= "/fans.html";
            int length = strlen(pFans);
            strncpy(m_real_file_path+len,pFans,length);
        }
        else
        {
            strncpy(m_real_file_path+len,m_url,FILENAME_MAX-len-1);
        }
        //通过stat函数获取请求的资源的属性信息
        if(stat(m_real_file_path,&m_file_stat)<0)
        {
            return NO_REQUEST;
        }
 
        //文件类型是目录
        if(S_ISDIR(m_file_stat.st_mode))
        {
            return BAD_REQUEST;
        }
        //以只读方式获取文件描述符，通过mmap将该文件映射到内存中
        int fd=::open(m_real_file_path,O_RDONLY);
        m_file_address=(char*)mmap(0,m_file_stat.st_size,PROT_READ,MAP_PRIVATE,fd,0);
 
        //避免文件描述符的浪费和占用
        close(fd);
 
        //表示请求文件存在，且可以访问
        return FILE_REQUEST;
 
 
    }
    bool Http_Data::ConstructResponse(HTTP_STATE ret)
    {
        //http响应报文格式
        /*
        状态行：版本\t状态码\t状态信息\r\n   //add_status_line:
        首部行
        空 行：\r\n
        实体体
        */
        switch(ret)
        {
            case INTERNAL_ERROR:
            {
                //内部错误 500
                AddStatusLine(500,error_500_title);
                AddHeaders(strlen(error_500_title));
                if(!AddContent(error_500_form))
                {
                    return false;
                }
                break;
            }
        
            case BAD_REQUEST:
            {
                //报文语法有误 404
                AddStatusLine(404,error_404_title);
                AddHeaders(strlen(error_404_title));
                if(!AddContent(error_404_form))
                {
                    return false;
                }
                break;
            }
            case FORBIDDEN_REQUEST:
            {
                //没有访问权限  403
                 //报文语法有误 404
                AddStatusLine(403,error_403_title);
                AddHeaders(strlen(error_403_title));
                if(!AddContent(error_403_form))
                {
                    return false;
                }
                break;
            }
            case FILE_REQUEST:
            {
                AddStatusLine(200,ok_200_title);
                if(m_file_stat.st_size!=0)
                {
                    //使用IO向量机制，第一个iovec指向m_write_buff,存储的是相应报文的状态行、首部行、空行
                    //第二个iovec指向请求的文件所映射到的地址
                    AddHeaders(m_file_stat.st_size);
                    m_write_buff.Append(m_file_address,m_file_stat.st_size);                  
                    return true;
        
        
                }
                else
                {
                    //如果请求的资源大小为0，则返回空白html文件
                    const char* ok_string="<html><body></body></html>";
                    AddHeaders(strlen(ok_string));
                    if(!AddContent(ok_string))
                        return false;
                }
                break;
            }
            return true;
        }
    }

    bool Http_Data::AddResponse(const char* format,...)
    {
        char tmp_buff[1024];
        va_list valist;
        va_start(valist,format);
        int len = vsnprintf(tmp_buff,1024,format,valist);
        va_end(valist);

        m_write_buff.Append(tmp_buff,len);
        return true;
    }

    bool Http_Data::AddStatusLine(int status,const char* title)
    {
        AddResponse("%s %d %s \r\n","HTTP/1.1",status,title);
    }

    bool Http_Data::AddHeaders(int content_len)
    {
        AddContentLength(content_len);
        AddContentType();
        AddConnection();
        AddBlankLine();
    }

    bool Http_Data::AddContentLength(int content_len)
    {
        AddResponse("Content-Length:%d\r\n",content_len);
    }
    bool Http_Data::AddContentType()
    {
        AddResponse("Content-Type:%s\r\n","Text/Html");
    }
    bool Http_Data::AddConnection()
    {
        if(m_linger)
        {
            AddResponse("Conectino:%s\r\n","keep-alive");
        }
        else
        {
            AddResponse("Conectino:%s\r\n","close");
        }
    }

    bool Http_Data::AddBlankLine()
    {
         AddResponse("%s","\r\n");
    }

    bool Http_Data::AddContent(const char* pContent)
    {
        bool res = AddResponse("%s",pContent);
        return res;
    }


}