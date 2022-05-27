
#include <unistd.h>

#include "Channel.h"
#include "EventLoop.h"
#include "TcpClient.h"
#include "TcpConnection.h"
#include "InetAddress.h"

#include "../base/Log.h"
namespace afa
{
    static Logger::Ptr logger = LOG_ROOT();
    TcpClient::TcpClient(EventLoop* loop,const InetAddress& address)
    :m_loop(loop)
    ,m_socketfd(socket(AF_INET,SOCK_STREAM,0))
    ,m_peer_address(address)
    {
        assert(m_socketfd>0);
    }
    TcpClient::~TcpClient()
    {
        m_sp_conn->CloseHandle();
        close(m_socketfd);
    }

    int TcpClient::Connect()
    {
        LOG_DEBUG(logger)<<"Connecting to "<<m_peer_address.StrilizeIpAddress()<<" at port:"<<m_peer_address.Port();
        //int sockfd = socket(AF_INET,SOCK_STREAM,0);
        //int n = connect(sockfd,(struct sockaddr*)(&m_peer_address),sizeof(m_peer_address));
        int ret = TcpSocket::Connect(m_socketfd,m_peer_address);
        if(ret<0)
        {
            return -1;
        }
        m_sp_conn.reset(new TcpConnection(m_loop,m_socketfd,m_peer_address));
        m_sp_conn->SetMessageCallBack(m_messagecb);
        m_loop->RunInLoop(std::bind(&TcpConnection::ConnectionEstablished,m_sp_conn));
        struct linger ling;
        ling.l_onoff=1;
        ling.l_linger = 0;
        setsockopt(m_socketfd,SOL_SOCKET,SO_LINGER,&ling,static_cast<socklen_t>(sizeof(ling)));
        return 0;
    }

    void TcpClient::SetMessageCallback(const MessageCallback &message_cb)
    {
        m_messagecb = message_cb;
    }
}