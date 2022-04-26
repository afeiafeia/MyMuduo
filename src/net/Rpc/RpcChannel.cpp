#include "RpcChannel.h"
#include <functional>

namespace afa
{
    RpcChannel::RpcChannel()
    {

    }

    RpcChannel::RpcChannel(SP_TcpConnection &sp_conn)
    :m_sp_conn(sp_conn)
    {
        m_codec.SetMessageCallBack(std::bind(&RpcChannel::OnRpcMessage,this,std::placeholders::_1,std::placeholders::_2));
    }

    RpcChannel::~RpcChannel()
    {

    }

    void RpcChannel::CallMethod(
        const google::protobuf::MethodDescriptor* method,
        google::protobuf::RpcController* controller, 
        const google::protobuf::Message* request,
        google::protobuf::Message* response, 
        google::protobuf::Closure* done)
    {
        //将实际的消息（request）以及调用的方法（method）封装到RpcMessage中发送
        RpcMessage cur_msg;
        cur_msg.set_type(MessageType::REQUEST);
        cur_msg.set_id(++m_id);
        cur_msg.set_service(method->service()->full_name());
        cur_msg.set_method(method->name());
        cur_msg.set_request(request->SerializeAsString());

        m_codec.Send(m_sp_conn,cur_msg);
    }

    void RpcChannel::OnMessage(SP_TcpConnection sp_conn,Buffer &buf)
    {
        m_codec.OnMessage(sp_conn,buf);
    }

    void RpcChannel::OnRpcMessage(SP_TcpConnection sp_conn,std::shared_ptr<google::protobuf::Message> sp_msg)
    {
        google::protobuf::Message* request = nullptr;
        google::protobuf::Message* response = nullptr;
        std::shared_ptr<RpcMessage> sp_rpc_msg = std::dynamic_pointer_cast<RpcMessage>(sp_msg);
        if(sp_rpc_msg==nullptr)
        {
            //错误处理
            m_error = ErrorCode::INVALID_REQUEST;
        }
        else if(sp_rpc_msg->type()==MessageType::REQUEST)
        {
            std::map<std::string,google::protobuf::Service*>::iterator it = m_map_nametoservice.find(sp_rpc_msg->service());
            if(it!=m_map_nametoservice.end())
            {
                google::protobuf::Service* service = it->second;
                const google::protobuf::ServiceDescriptor* service_desc = service->GetDescriptor();
                const google::protobuf::MethodDescriptor* method = service_desc->FindMethodByName(sp_rpc_msg->method());
                if(method)
                {
                    request = service->GetRequestPrototype(method).New();
                    request->ParseFromString(sp_rpc_msg->request());
                    response = service->GetResponsePrototype(method).New();
                    service->CallMethod(method,nullptr,request,response,nullptr);
                    m_error = ErrorCode::NO_ERROR;
                }
                else
                {
                    //错误处理
                     m_error = ErrorCode::NO_METHOD;
                }

            }
            else
            {
                //错误处理
                m_error = ErrorCode::NO_SERVICE;
            }
        }
        else
        {
            //错误处理
            m_error = ErrorCode::INVALID_REQUEST;
        }

        if(m_error==ErrorCode::NO_ERROR)
        {
            RpcMessage msg;
            msg.set_type(MessageType::RESPONSE);
            msg.set_id(++m_id);
            msg.set_error(ErrorCode::NO_ERROR);
            msg.set_response(response->SerializeAsString());

            m_codec.Send(m_sp_conn,msg);

        }
    }

}