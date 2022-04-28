#include "RpcChannel.h"
#include <functional>
#include "../../base/Log.h"
namespace afa
{
    static Logger::Ptr logger = LOG_ROOT();
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
        LOG_INFO(logger)<<"Construct RpcMessage by ProtoMessage!";
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
        LOG_INFO(logger)<<"RpcChannel::OnMessage()";
        m_codec.OnMessage(sp_conn,buf);
    }

    void RpcChannel::OnRpcMessage(SP_TcpConnection sp_conn,std::shared_ptr<google::protobuf::Message> sp_msg)
    {
        LOG_INFO(logger)<<"RpcChannel::OnRpcMessage():回复报文！";
        google::protobuf::Message* request = nullptr;
        google::protobuf::Message* response = nullptr;
        std::shared_ptr<RpcMessage> sp_rpc_msg = std::dynamic_pointer_cast<RpcMessage>(sp_msg);
        if(sp_rpc_msg==nullptr)
        {
            //错误处理
            LOG_ERROR(logger)<<"向下转型失败!";
            m_error = ErrorCode::INVALID_REQUEST;
        }
        else if(sp_rpc_msg->type()==MessageType::REQUEST)
        {
            LOG_ERROR(logger)<<"请求类型报文处理!";
            std::string service_name = sp_rpc_msg->service(); 
            LOG_ERROR(logger)<<"服务名称"<<service_name;
            std::map<std::string,google::protobuf::Service*>::iterator it = m_map_nametoservice.find(sp_rpc_msg->service());
            if(it!=m_map_nametoservice.end())
            {
                LOG_ERROR(logger)<<"找到指定服务!";
                google::protobuf::Service* service = it->second;
                const google::protobuf::ServiceDescriptor* service_desc = service->GetDescriptor();
                const google::protobuf::MethodDescriptor* method = service_desc->FindMethodByName(sp_rpc_msg->method());
                if(method)
                {
                    LOG_ERROR(logger)<<"找到服务的对应Method!";
                    request = service->GetRequestPrototype(method).New();
                    request->ParseFromString(sp_rpc_msg->request());
                    response = service->GetResponsePrototype(method).New();
                    service->CallMethod(method,nullptr,request,response,nullptr);
                    m_error = ErrorCode::NO_ERROR;
                }
                else
                {
                    //错误处理
                    LOG_ERROR(logger)<<"未找到服务的对应Method!";
                    m_error = ErrorCode::NO_METHOD;
                }

            }
            else
            {
                //错误处理
                LOG_ERROR(logger)<<"找到指定服务!";
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
             LOG_ERROR(logger)<<"处理成功，回复相应报文!";
            RpcMessage msg;
            msg.set_type(MessageType::RESPONSE);
            msg.set_id(++m_id);
            msg.set_error(ErrorCode::NO_ERROR);
            msg.set_response(response->SerializeAsString());

            m_codec.Send(m_sp_conn,msg);

        }
    }

}