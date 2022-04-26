#ifndef RPC_SERVER_H
#define RPC_SERVER_H

#include "../TcpServer.h"

#include <memory>
#include <map>
#include <google/protobuf/service.h>
namespace afa
{
    class TcpConnection;
    class Buffer;
    class EvewntLoop;
    class RpcChannel;

    class RpcServer
    {
    public:
        typedef std::shared_ptr<TcpConnection> SP_TcpConnection;
    public:
        RpcServer(EventLoop* loop,int32_t port,int threadNum = 5);
        ~RpcServer();

        void Start();

        void OnConnection(SP_TcpConnection sp_conn);

        void RegisterService(google::protobuf::Service* service);
    private:
        TcpServer m_server;
        std::map<int,std::shared_ptr<RpcChannel>> m_map_fdtorpcchannel;
        std::map<std::string,google::protobuf::Service*> m_map_nametoservice;

    };

}

#endif