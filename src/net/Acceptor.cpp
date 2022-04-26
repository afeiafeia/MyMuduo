#include "Acceptor.h"
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <assert.h>
#include <unistd.h>
#include "../base/Log.h"
namespace afa
{
    static Logger::Ptr logger = LOG_ROOT();

    Acceptor::Acceptor(int port,EventLoop* loop)
    :m_loop(loop)
    ,m_listenfd(socket(AF_INET,SOCK_STREAM,0))
    ,m_port(port)
    ,m_channel(new Channel(m_listenfd,m_loop))
    {
        LOG_DEBUG(logger)<<"Creating listen fd: "<<m_listenfd;
        m_channel->SetReadHandle(std::bind(&Acceptor::NewAccept,this));
        int optival = 1;
        //地址复用
        setsockopt(m_listenfd,SOL_SOCKET,SO_REUSEADDR,&optival,static_cast<socklen_t>(sizeof optival));
        //端口复用
        setsockopt(m_listenfd,SOL_SOCKET,SO_REUSEPORT,&optival,static_cast<socklen_t>(sizeof optival));
    }
    Acceptor::~Acceptor()
    {

    }
    void Acceptor::Listen()
    {
        struct sockaddr_in address;
        bzero(&address,sizeof(sockaddr_in));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_ANY);
        address.sin_port = m_port;
        int ret = bind(m_listenfd,(struct sockaddr*)(&address),sizeof(address));
        assert(ret>=0);//如果为真继续执行，如果为假程序退出
        ret = listen(m_listenfd,100);
        assert(ret>=0);

        m_channel->EnableReading();
        LOG_DEBUG(logger)<<"start to listen!";;
    }

    void Acceptor::NewAccept()
    {
        if(m_channel->IsETTrigger())
        {
            while(true)
            {
                //非阻塞读取：一般的accept没有最后一个选项，新函数accept4有此参数
                bool res = AcceptConnection();
                if(res==false)
                {
                    break;
                }
            }
        }
        else
        {
            AcceptConnection();
        }   
    }

    bool Acceptor::AcceptConnection()
    {
        struct sockaddr_in src_dsct;
        socklen_t length = static_cast<socklen_t>(sizeof(src_dsct));
        int connfd = accept4(m_listenfd,(struct sockaddr*)&src_dsct,&length,SOCK_NONBLOCK);
        if(connfd>0)
        {
            LOG_DEBUG(logger)<<"New connection: "<<connfd<<" is coming!";
            //如果设置了回调函数，执行回调函数，否则关闭连接套接字不予处理
            if(m_new_connection_CB)
            {
                m_new_connection_CB(connfd,(struct sockaddr*)&src_dsct,length);
            }
            else
            {
                close(connfd);
                return false;
            }

        }
        else
        {
            if(errno!=EAGAIN&&errno==EWOULDBLOCK)
            {
                LOG_ERROR(logger)<<"Failed to accept new connection!\n";
            }
            return false;
        }
        return true;
    }

}