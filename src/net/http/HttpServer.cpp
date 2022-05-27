#include "HttpServer.h"
#include "HttpData.h"
#include "../Channel.h"
#include "../../base/Log.h"
#include <unistd.h>
#include "../TcpConnection.h"
namespace afa
{
    static Logger::Ptr logger = LOG_ROOT();
    HttpServer::HttpServer(EventLoop* loop,uint32_t port)
    :m_server(loop,port)
    {
        m_server.SetMessageCallBack(std::bind(&HttpServer::OnMessage,this,std::placeholders::_1,std::placeholders::_2));
        m_server.SetConnectionCallBack(std::bind(&HttpServer::OnConnection,this,std::placeholders::_1));
        m_server.SetCloseCallBack(std::bind(&HttpServer::CloseConnection,this,std::placeholders::_1));
    }

    HttpServer::~HttpServer()
    {

    }

 

    //void HttpServer::NewConnectionHandle(int sockfd,sockaddr* addr,socklen_t len)
    //{
    //    EventLoop* curLoop = m_spThreadPool->GetNextLoop();
    //    std::shared_ptr<HttpData> spHttpData(new HttpData(curLoop,sockfd));
    //    spHttpData->SetCloseCallBack(std::bind(HttpServer::CloseConnection,this,_1));
    //    m_vctHttpData.push_back(spHttpData);
//
    //    //连接套接字设置为非阻塞
    //    SetNonBlock(sockfd);
    //    
    //    //bind绑定智能指针时，不是将智能指针转换为普通指针，而是会隐式创建一个智能指针与bind绑定在一起，所以，即使此函数结束，spHttpData离开其作用域，所指资源也不会释放
    //    //但是，该函数在loop中执行完后，资源会释放，这将导致channel的回调出错（回调绑定了spHttpData所指对象的this指针，如果资源释放，this指针将指向非法地址）
    //    curLoop->RunInLoop(std::bind(&Http_Data::Enable,spHttpData));
//
    //    //保留副本，防止资源提前释放
    //    m_vctHttpData.push_back(spHttpData);
    //    m_mapHttpData[sockfd] = spHttpData;        
    //}
//
    //void HttpServer::CloseConnection(std::shared_ptr<Http_Data> spHttpData)
    //{
    //    m_loop->assertInLoopThread();
    //    m_server.Close
    //}

    void HttpServer::OnMessage(SP_TcpConnection sp_conn,Buffer &buff)
    {
        
        //while(buff.ReadableBytes()>0)
        {
            //使用循环的目的：如果缓冲区中有多条消息，可以一直处理直到处理完或者剩下的是不完整消息
            //int32_t length = buff.GetHeader32();
            //if(buff.ReadableBytes()>=static_cast<size_t>(length))
            {
                LOG_DEBUG(logger)<<"存在完整消息";
                //至少有一个完整消息
                //LOG_DEBUG(logger)<<"Data's length: "<<len;
                //LOG_DEBUG(logger)<<std::string(buff.Peek(),len);
                std::shared_ptr<Http_Data> data;
                int fd = sp_conn->GetChannel()->Getfd();
                if(m_mapHttpData.find(fd)!=m_mapHttpData.end())
                {
                    data = m_mapHttpData[fd];
                    data->AppendData(buff);

                }
                else
                {
                    m_mapHttpData[fd].reset(new Http_Data(buff));
                    data = m_mapHttpData[fd];
                }
                //buff.Retrieve(length);
                buff.RetrieveAll();
                if(data->Parse())
                {
                    Buffer &&buf = data->GetResponse();
                    sp_conn->Send(buf);
                }
                else
                {
                    //解析失败不一定是报文错误，也可能报文不完整
                    LOG_ERROR(logger)<<"解析失败";
                    //break;
                }
            }
            //else
            //{
            //    //是一个不完整消息
            //    LOG_INFO(logger)<<"待解析消息不完整";
            //   // break;
            //}
        }

        return;
    }

    void HttpServer::OnConnection(SP_TcpConnection sp_conn)
    {

    }

    void HttpServer::CloseConnection(SP_TcpConnection sp_conn)
    {
        int sockfd = sp_conn->GetChannel()->Getfd();
        {
            MutexLockGuard guard(&m_lock);
            m_mapHttpData.erase(sockfd);
        }
    }


}