#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "../../base/noncopyable.h"
#include "../../base/MutexLock.h"
#include "../../base/MutexLockGuard.h"
#include "../TcpServer.h"
#include <memory>
#include <map>
#include <vector>
namespace afa
{
    class EventLoop;
    class Channel;
    class Http_Data;
    class HttpServer:public noncopyable
    {
    public:
        typedef std::shared_ptr<Channel> SP_Channel;
        typedef std::shared_ptr<TcpConnection> SP_TcpConnection;
    private:
        EventLoop*    m_loop;
        TcpServer     m_server;
        std::vector<std::shared_ptr<Http_Data>> m_vctHttpData;
        std::map<int,std::shared_ptr<Http_Data>> m_mapHttpData;
        MutexLock                                m_lock;
    public:
        HttpServer(EventLoop* loop,uint32_t port);
        ~HttpServer();

        void Start()
        {
            m_server.Start();
        }

        void OnConnection(SP_TcpConnection sp_conn);
        void OnMessage(SP_TcpConnection sp_conn,Buffer &buff);

        void CloseConnection(SP_TcpConnection sp_conn);

        std::shared_ptr<Http_Data> Parse(SP_TcpConnection sp_conn,Buffer &buff);
    };
}


#endif