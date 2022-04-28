#include "RpcServer.h"
#include "RpcChannel.h"
#include "../TcpConnection.h"
#include "../../base/Log.h"
#include <google/protobuf/descriptor.h>
#include <functional>
namespace afa
{
    static Logger::Ptr logger = LOG_ROOT();

    RpcServer::RpcServer(EventLoop* loop,int32_t port,int threadNum)
    :m_server(loop,port,threadNum)
    {
        m_server.SetConnectionCallBack(std::bind(&RpcServer::OnConnection,this,std::placeholders::_1));
    }
    RpcServer::~RpcServer()
    {

    }

    void RpcServer::Start()
    {
        LOG_DEBUG(logger)<<"RpcServer is starting";
        m_server.Start();
    }
    void RpcServer::RegisterService(google::protobuf::Service* service)
    {
        const google::protobuf::ServiceDescriptor* descriptor = service->GetDescriptor();
        std::string name = descriptor->full_name();
        m_map_nametoservice[name] = service;
        LOG_DEBUG(logger)<<"Registering service: "<<name;
    }


    void RpcServer::OnConnection(SP_TcpConnection sp_conn)
    {
        std::shared_ptr<RpcChannel> sh_rpc_channel(new RpcChannel(sp_conn));
        m_map_fdtorpcchannel[sp_conn->GetChannel()->Getfd()] = sh_rpc_channel;
        sh_rpc_channel->SetRegisterServices(m_map_nametoservice);
        sp_conn->SetMessageCallBack(std::bind(&RpcChannel::OnMessage,sh_rpc_channel.get(),std::placeholders::_1,std::placeholders::_2));
    }
}