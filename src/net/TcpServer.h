#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "Buffer.h"
#include "Channel.h"
#include "TimerManager.h"
#include <functional>
#include <map>
#include <memory>
#include <sys/socket.h>
namespace afa
{
    class EventLoop;
    class Acceptor;
    class EventLoopThreadPool;
    class TcpConnection;

    class TcpServer
    {
    public:
        typedef std::function<void (const std::shared_ptr<TcpConnection>)> ConnectionCallback;
        typedef std::function<void (const std::shared_ptr<TcpConnection>)> CloseCallback;
        typedef std::function<void (const std::shared_ptr<TcpConnection>,Buffer &)> MessageCallBack;
        typedef std::shared_ptr<TcpConnection> SP_TcpConnection;

    public:


    private:
        EventLoop*                           m_loop;                //所在loop
        std::shared_ptr<Acceptor>            m_sp_acceptor;         //监听者
        int                                  m_thread_num;          //线程池的线程数
        std::shared_ptr<EventLoopThreadPool> m_sp_threadpool;        //线程池对象
        ConnectionCallback                   m_connection_callBack; //连接回调
        MessageCallBack                      m_message_callBack;    //消息回调：服务器对应业务处理
        CloseCallback                        m_close_callback;      //关闭连接时的回调

        std::map<int,std::shared_ptr<TcpConnection>> m_map_connection; //TCP连接对象的智能指针

        Channel::TriggerModel                m_listen_model;       //监听套接字的监听模式
        Channel::TriggerModel                m_sock_model;         //连接套接字的监听模式


    public:
        TcpServer(EventLoop* loop,int32_t port,int threadNum = 5);
        ~TcpServer();

        void Start();

        void SetAcceptorETTrigger();
        void SetConnETTrigger();
        
        void SetConnectionCallBack(const ConnectionCallback &cb)
        {
            m_connection_callBack = cb;
        }

        void SetMessageCallBack(const MessageCallBack &cb)
        {
            m_message_callBack = cb;
        }

        void SetCloseCallBack(const CloseCallback &cb)
        {
            m_close_callback = cb;
        }
    private:


        void NewConnectionHandle(int sockfd,sockaddr* addr,socklen_t len);

        void CloseConnection(std::shared_ptr<TcpConnection> spConn);

        int SetNonBlock(int fd);
    };

}




#endif