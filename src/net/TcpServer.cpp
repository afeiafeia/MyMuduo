#include "TcpServer.h"
#include "Acceptor.h"
#include "EventLoopThreadPool.h"
#include "TcpConnection.h"
#include <sys/fcntl.h>
#include "../base/Log.h"
namespace afa
{
    static Logger::Ptr logger = LOG_ROOT();
    TcpServer::TcpServer(EventLoop* loop,int32_t port,int thread_num)
    :m_loop(loop)
    ,m_sp_acceptor(new Acceptor(port,loop))
    ,m_thread_num(thread_num)
    ,m_sp_threadpool(new EventLoopThreadPool(loop,thread_num))
    {
        m_sp_acceptor->SetNewConnectionCB(
            std::bind(&TcpServer::NewConnectionHandle,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    }
    TcpServer::~TcpServer()
    {

    }

    void TcpServer::Start()
    {
        m_sp_threadpool->Start();
        if(m_listen_model==Channel::TriggerModel::ET)
        {
            m_sp_acceptor->GetChannel()->SetETTrigger();
            LOG_DEBUG(logger)<<"Setting listenfd: "<<m_sp_acceptor->GetChannel()->Getfd()<<" model to ET!";
        }
        m_sp_acceptor->Listen();
    }

    void TcpServer::SetAcceptorETTrigger()
    {
        m_listen_model = Channel::TriggerModel::ET;
    }

    void TcpServer::SetConnETTrigger()
    {
        m_sock_model = Channel::TriggerModel::ET;
    }

    int TcpServer::SetNonBlock(int fd)
    {
        int old_option = fcntl(fd,F_GETFD);
        int new_option = old_option|O_NONBLOCK;
        fcntl(fd,F_SETFD,new_option);
        return old_option;
    }

    void TcpServer::NewConnectionHandle(int sockfd,sockaddr* addr,socklen_t len)
    {
        EventLoop* cur_loop = m_sp_threadpool->GetNextLoop();
        SP_TcpConnection sp_conn(new TcpConnection(cur_loop,sockfd,addr,len));
        if(m_sock_model==Channel::TriggerModel::ET)
        {
            sp_conn->GetChannel()->SetETTrigger();
        }
        sp_conn->SetConnectionCallBack(m_connection_callBack);
        sp_conn->SetMessageCallBack(m_message_callBack);
        sp_conn->SetCloseCallBack(std::bind(&TcpServer::CloseConnection,this,std::placeholders::_1));

        //连接套接字设置为非阻塞
        SetNonBlock(sockfd);
        
        //bind绑定智能指针时，不是将智能指针转换为普通指针，而是会隐式创建一个智能指针与bind绑定在一起，所以，即使此函数结束，spHttpData离开其作用域，所指资源也不会释放
        //但是，该函数在loop中执行完后，资源会释放，这将导致channel的回调出错（回调绑定了spHttpData所指对象的this指针，如果资源释放，this指针将指向非法地址）
        cur_loop->RunInLoop(std::bind(&TcpConnection::ConnectionEstablished,sp_conn));
        
        //保留副本，防止资源提前释放
        m_map_connection[sockfd] = sp_conn;        
    }

    void TcpServer::CloseConnection(std::shared_ptr<TcpConnection> sp_conn)
    {
        //作为TcpConnection关闭时的回调函数，移除TcpServer所持有的智能指针，否则TcpConnection关闭后，仍占有着资源
        if(m_close_callback)
        {
            m_close_callback(sp_conn);
        }
        size_t n = m_map_connection.erase(sp_conn->GetChannel()->Getfd());
        (void)n;
        assert(n == 1);
    }

   
}