#ifndef ECHO_SERVER_H
#define ECHO_SERVER_H
#include "TcpServer.h"
#include "Buffer.h"
#include <memory>
namespace afa
{
    class EventLoop;
    class TcpConnection;
    class EchoServer
    {
    private:
        TcpServer  m_server;
        EventLoop* m_loop;

    public:
        EchoServer(EventLoop* loop,uint32_t port,int threadNum=5);

        void Start();

        void Connection(const std::shared_ptr<TcpConnection>& conn);

        void Message(const std::shared_ptr<TcpConnection> &conn,Buffer &message);
    };
}
#endif