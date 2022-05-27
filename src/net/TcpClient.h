#ifndef TCP_CLINET_H
#define TCP_CLINET_H

#include <functional>
#include <memory>
#include <netinet/in.h>

#include "Buffer.h"
#include "Callback.h"
#include "TcpSocket.h"
#include "../base/noncopyable.h"
namespace afa
{
    class EventLoop;
    class TcpConnection;
    class TcpClient:public noncopyable
    {
    public:
        typedef std::shared_ptr<TcpConnection> SP_TcpConnection;
    private:
        EventLoop*       m_loop;
        int              m_socketfd;      //连接套接字
        InetAddress      m_peer_address;  //对端地址
        SP_TcpConnection m_sp_conn;       //连接
        MessageCallback  m_messagecb;     //套接字可读时的回调函数

    public:
        TcpClient(EventLoop* loop,const InetAddress& address);
        ~TcpClient();

        //建立连接
        int Connect();

        void SetMessageCallback(const MessageCallback &message_cb);

    };
}

#endif