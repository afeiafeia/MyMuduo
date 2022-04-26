#include "HttpServer.h"
#include "HttpData.h"
#include "../Channel.h"
#include <unistd.h>
namespace afa
{
    HttpServer::HttpServer(EventLoop* loop,uint32_t port)
    :m_server(loop,port)
    {
        m_server.SetConnHandle(std::bind(&HttpServer::NewConnectionHandle,this,_1,_2,_3));
    }

    HttpServer::~HttpServer()
    {

    }

 

    void HttpServer::NewConnectionHandle(int sockfd,sockaddr* addr,socklen_t len)
    {
        EventLoop* curLoop = m_spThreadPool->GetNextLoop();
        std::shared_ptr<HttpData> spHttpData(new HttpData(curLoop,sockfd));
        spHttpData->SetCloseCallBack(std::bind(HttpServer::CloseConnection,this,_1));
        m_vctHttpData.push_back(spHttpData);

        //连接套接字设置为非阻塞
        SetNonBlock(sockfd);
        
        //bind绑定智能指针时，不是将智能指针转换为普通指针，而是会隐式创建一个智能指针与bind绑定在一起，所以，即使此函数结束，spHttpData离开其作用域，所指资源也不会释放
        //但是，该函数在loop中执行完后，资源会释放，这将导致channel的回调出错（回调绑定了spHttpData所指对象的this指针，如果资源释放，this指针将指向非法地址）
        curLoop->RunInLoop(std::bind(&Http_Data::Enable,spHttpData));

        //保留副本，防止资源提前释放
        m_vctHttpData.push_back(spHttpData);
        m_mapHttpData[sockfd] = spHttpData;        
    }

    void HttpServer::CloseConnection(std::shared_ptr<Http_Data> spHttpData)
    {
        m_loop->assertInLoopThread();
        size_t n = m_mapHttpData.erase(spHttpData->GetChannel()->Getfd());
        (void)n;
        assert(n == 1);
    }
}