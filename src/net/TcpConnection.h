#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#define READ_BUFF_SIZE 1024
#define WRITE_BUFF_SIZE 1024
#include "Buffer.h"
#include <memory>
#include <functional>
#include <string>
#include <sys/socket.h>
namespace afa
{
    class EventLoop;
    class Channel;
    class TcpConnection:public std::enable_shared_from_this<TcpConnection>
    {
    public:
        typedef std::shared_ptr<Channel> SP_Channel;
        typedef std::function<void(const std::shared_ptr<TcpConnection>)> CloseCallBack;
        typedef std::function<void (const std::shared_ptr<TcpConnection>)> ConnectionCallback;
        typedef std::function<void (const std::shared_ptr<TcpConnection>,Buffer&)> MessageCallback;
    private:
        enum State
        {
            Connecting=0,
            Connected,
            DisConnected,
        };
  
        EventLoop*               m_loop;
        std::shared_ptr<Channel> m_sp_channel;
        struct sockaddr*         m_addr;
        socklen_t                m_addr_len;
        State                    m_state;

        //char m_read_buff[READ_BUFF_SIZE]; //读缓冲区（套接字可读时，将数据读到此缓冲区）
        //int  m_read_index;                 //读取到的字节数，而不是最后一个字节的下标
        //std::string m_read_message;
//
        //char m_write_buff[WRITE_BUFF_SIZE];
        //int  m_write_index;
        //std::string m_write_message;

        Buffer  m_read_buff;
        Buffer  m_write_buff;

        CloseCallBack m_close_callBack;  //tcpConnection被server持有，该函数用于在server中移除对TcpConnection的持有
        MessageCallback m_message_callBack;//逻辑处理
        ConnectionCallback m_connection_callBack;//用于输出一些基本信息

    public:
        TcpConnection(EventLoop* loop,int fd,sockaddr* addr,socklen_t len);
        ~TcpConnection();

        void SetCloseCallBack(const CloseCallBack &cb)
        {
            m_close_callBack = cb;
        }

        void SetMessageCallBack(const MessageCallback &cb)
        {
            m_message_callBack = cb;
        }

        void SetConnectionCallBack(const ConnectionCallback &cb)
        {
            m_connection_callBack = cb;
        }


        void SetState(State state)
        {
            m_state = state;
        }

        SP_Channel GetChannel()
        {
            return m_sp_channel;
        }


        void ReadHandle();
        void WriteHandle();
        void ErrorHandle();
        void CloseHandle();

        void ConnectionEstablished();

        void ConnectionDestroyed();

        void Send(std::string &msg);

        void Send(Buffer &buf);




    };
}
typedef std::shared_ptr<afa::TcpConnection> SP_TcpConnection;
#endif