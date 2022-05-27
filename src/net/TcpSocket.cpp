#include "TcpSocket.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
namespace afa
{
    TcpSocket::TcpSocket(int fd,bool nonblock)
    :m_sockfd(fd)
    {
        if(nonblock)
        {
            SetNonBlock();
        }
    }
    TcpSocket::~TcpSocket()
    {
    }
    int TcpSocket::Connect(int sockfd,const InetAddress &peer_address)
    {
        sockaddr_in &&address = peer_address.SockAddr();
        int fd = connect(sockfd,(struct sockaddr*)&address,sizeof(address));
        return fd;
    }

    int TcpSocket::Accept(InetAddress &peer_address)
    {
        sockaddr_in address;
        bzero(&address,sizeof(address));
        socklen_t len=static_cast<socklen_t>(sizeof(address));
        int fd = accept(m_sockfd,(struct sockaddr*)&address,&len);
        peer_address.SetSockAddr(address);
        return fd;
    }

    int TcpSocket::Bind(const InetAddress &listen_address)
    {
        sockaddr_in &&address = listen_address.SockAddr();
        int ret = bind(m_sockfd,(struct sockaddr*)&address,sizeof(address));
        return ret;
    }

    int TcpSocket::Listen(int backlog)
    {
        int ret = listen(m_sockfd,backlog);
        return ret;
    }

    int TcpSocket::SetNonBlock()
    {
        int oldoption = fcntl(m_sockfd,F_GETFD);
        int newoption = oldoption|O_NONBLOCK;
        fcntl(m_sockfd,F_SETFD,newoption);
        return oldoption;
    }

    void TcpSocket::SetReuseAddr(bool on)
    {
        int optival = on?1:0;
        setsockopt(m_sockfd,SOL_SOCKET,SO_REUSEADDR,&optival,static_cast<socklen_t>(sizeof(optival)));
    }

    void TcpSocket::SetReusePort(bool on)
    {
        int optival = on?1:0;
        setsockopt(m_sockfd,SOL_SOCKET,SO_REUSEPORT,&optival,static_cast<socklen_t>(sizeof optival));
    }

    void TcpSocket::SetKeepAlive(bool on)
    {
        int optival = on?1:0;
        setsockopt(m_sockfd,SOL_SOCKET,SO_KEEPALIVE,&optival,static_cast<socklen_t>(sizeof(optival)));
    }

    void TcpSocket::SetLinger(bool on,int delay)
    {
        struct linger lin;
        lin.l_onoff = on?1:0;
        lin.l_linger = on?delay:0;
        setsockopt(m_sockfd,SOL_SOCKET,SO_LINGER,&lin,static_cast<socklen_t>(sizeof(lin)));
    }

    
}