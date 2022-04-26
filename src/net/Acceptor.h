#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include <functional>
#include <memory>
#include <sys/socket.h>
#include "Channel.h"
class EventLoop;
namespace afa
{
    class Acceptor
    {
    public:
        typedef std::function<void(void*)> CallBack;
        typedef std::function<void (int sockfd,sockaddr* addr,socklen_t len)> NewConnectionCallBack;

    private:
        EventLoop*               m_loop;
        int                      m_listenfd;  //监听套接字
        int32_t                  m_port;      //所监听端口
        std::unique_ptr<Channel> m_channel;   //监听套接字对应的channel
        NewConnectionCallBack    m_new_connection_CB; //先连接到来时调用的函数，对连接套接字处理

    public:
        Acceptor(int32_t port,EventLoop* loop);
        ~Acceptor();

        void Listen();

        const std::unique_ptr<Channel> &GetChannel()
        {
            return m_channel;
        }


        void SetNewConnectionCB(const NewConnectionCallBack &cb)
        {
            m_new_connection_CB = cb;
        }

    private:
        void NewAccept();

        bool AcceptConnection();

    };
}


#endif