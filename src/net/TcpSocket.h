#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H
#include <netinet/in.h>
#include "InetAddress.h"
#include "../base/noncopyable.h"
namespace afa
{
    class TcpSocket:public noncopyable
    {
    private:
        int m_sockfd;
    public:
        explicit TcpSocket(int fd,bool nonblock=true);
        ~TcpSocket();
        int Accept(InetAddress &peer_address);

        int Bind(const InetAddress &listen_address);

        int Listen(int backlog);

        int Getfd()const
        {
            return m_sockfd;
        }

        void SetReuseAddr(bool on);
        void SetReusePort(bool on);

        void SetKeepAlive(bool on);

        void SetLinger(bool on,int delay);

        static int Connect(int sockfd,const InetAddress &peer_address);

    private:
        int SetNonBlock();
    };
}


#endif