
#include "EchoServer.h"
#include "TcpConnection.h"
#include "Channel.h"
#include "../base/Log.h"
namespace afa
{
    static Logger::Ptr logger = LOG_ROOT();
    EchoServer::EchoServer(EventLoop* loop,uint32_t port,int threadNum)
    :m_loop(loop)
    ,m_server(loop,port,threadNum)
    {
        m_server.SetConnectionCallBack(std::bind(&EchoServer::Connection,this,std::placeholders::_1));
        m_server.SetMessageCallBack(std::bind(&EchoServer::Message,this,std::placeholders::_1,std::placeholders::_2));
    }

    void EchoServer::Start()
    {
        m_server.Start();
    }

    void EchoServer::Connection(const std::shared_ptr<TcpConnection>& conn)
    {
        LOG_DEBUG(logger)<<"On Connection!";
    }

    void EchoServer::Message(const std::shared_ptr<TcpConnection> &conn,Buffer &message)
    {
        LOG_DEBUG(logger)<<"Send Message!";
        std::string msg(message.RetrieveAllAsString());
        conn->Send(msg);
    }
}