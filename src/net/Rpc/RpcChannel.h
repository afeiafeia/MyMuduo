#ifndef RPC_CHANNEL_H
#define RPC_CHANNEL_H

//protobuf
#include <google/protobuf/service.h>
//内部
#include "../protobuf/CodeC.h"
#include "../TcpConnection.h"
#include "rpc.pb.h"

//C++
#include <memory>

namespace afa
{
    class RpcChannel:public::google::protobuf::RpcChannel
    {
        typedef std::shared_ptr<TcpConnection> SP_TcpConnection;
        typedef std::shared_ptr<google::protobuf::Message> SP_Message;
    public:
        RpcChannel();
        RpcChannel(SP_TcpConnection &sp_conn);

        ~RpcChannel();

        void OnMessage(SP_TcpConnection sp_conn,Buffer &buf);

        void OnRpcMessage(SP_TcpConnection sp_conn,std::shared_ptr<google::protobuf::Message> sp_msg);

        virtual void CallMethod(
            const google::protobuf::MethodDescriptor* method,
            google::protobuf::RpcController* controller, 
            const google::protobuf::Message* request,
            google::protobuf::Message* response, 
            google::protobuf::Closure* done);

        void SetRegisterServices(const std::map<std::string,google::protobuf::Service*> &map_services)
        {
            m_map_nametoservice = map_services;
        }
    private:
        std::atomic_uint64_t m_id;
        ErrorCode m_error;
        ProtobufCodeC m_codec;
        SP_TcpConnection m_sp_conn;
        std::map<std::string,google::protobuf::Service*> m_map_nametoservice;
    };
}

#endif