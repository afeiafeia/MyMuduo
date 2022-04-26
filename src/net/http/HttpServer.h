#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "../../noncopyable.h"
#include "../../MutexLock.h"
#include "../../MutexLockGuard.h"
#include "../Server.h"
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
    private:
        EventLoop* m_loop;
        Server     m_server;
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

        void NewConnectionHandle(int sockfd,sockaddr* addr,socklen_t len);

        void EraseHttpData(int sockfd)
        {
            MutexLockGuard guard(&m_lock);
            m_mapHttpData.erase(sockfd);
        }

        void CloseConnection(std::shared_ptr<Http_Data> spHttpData);
        void CloseConnection(std::shared_ptr<Http_Data> spHttpData);
    }
}


#endif