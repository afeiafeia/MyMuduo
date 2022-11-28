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
        RpcMessage curMsg;
        curMsg.set_type(MessageType::REQUEST);
        curMsg.set_id(++m_id);
        curMsg.set_service(method->service()->full_name());//服务的名称
        curMsg.set_method(method->name());//方法的名称
        curMsg.set_request(request->SerializeAsString());

        m_codec.Send(m_sp_conn,curMsg);
        m_Notify[m_id]=Semaphore(0);
        LOG_INFO(logger)<<"Starting wait for thread's notify";
        m_Notify[m_id].Wait();
        LOG_INFO(logger)<<"Main fun is notified!";
        m_Notify.erase(m_id);
    }

    void RpcChannel::OnMessage(SP_TcpConnection sp_conn,Buffer &buf)
    {
        LOG_INFO(logger)<<"RpcChannel::OnMessage()";
        m_codec.DecodeMessage(sp_conn,buf);
    }

    void RpcChannel::OnRpcMessage(SP_TcpConnection spTcpConn,std::shared_ptr<google::protobuf::Message> spMsg)
    {
        LOG_DEBUG(logger)<<"RpcChannel::OnRpcMessage():回复报文！";
        google::protobuf::Message* request = nullptr;
        google::protobuf::Message* response = nullptr;
        std::shared_ptr<RpcMessage> spRpcMsg = std::dynamic_pointer_cast<RpcMessage>(spMsg);
        if(spRpcMsg==nullptr)
        {
            //错误处理
            LOG_ERROR(logger)<<"向下转型失败!";
            m_error = ErrorCode::INVALID_REQUEST;
        }
        else if(spRpcMsg->type()==MessageType::REQUEST)
        {
            LOG_DEBUG(logger)<<"请求类型报文处理!";
            std::string serviceName = spRpcMsg->service(); 
            LOG_DEBUG(logger)<<"服务名称"<<serviceName;
            std::map<std::string,google::protobuf::Service*>::iterator it = m_mapNameToService.find(spRpcMsg->service());
            if(it!=m_mapNameToService.end())
            {
                LOG_ERROR(logger)<<"找到指定服务!";
                google::protobuf::Service* service = it->second;
                const google::protobuf::ServiceDescriptor* serviceDesc = service->GetDescriptor();
                const google::protobuf::MethodDescriptor* method = serviceDesc->FindMethodByName(spRpcMsg->method());
                if(method)
                {
                    LOG_DEBUG(logger)<<"找到服务的对应Method!";
                    request = service->GetRequestPrototype(method).New();
                    request->ParseFromString(spRpcMsg->request());
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
        else if(spRpcMsg->type()==MessageType::RESPONSE){
            LOG_DEBUG(logger)<<"This is response!";
            //如果是回复报文，spRpcMsg就是解析出的报文
            //调用对回复报文处理的函数：业务逻辑

            //int id = spRpcMsg->id();
            //m_mapResponse[id] = spRpcMsg;
            //m_Notify[id].Post();
        }
        else
        {
            //错误处理
            m_error = ErrorCode::INVALID_REQUEST;
        }

        //如果是请求报文，需要回复对端
        if(spRpcMsg->type()==MessageType::REQUEST){
            LOG_ERROR(logger)<<"处理成功，回复相应报文!";
            RpcMessage msg;
            int id = spRpcMsg->id();
            msg.set_type(MessageType::RESPONSE);
            msg.set_id(id);//保持id一致
            msg.set_error(m_error);
            msg.set_response(response->SerializeAsString());
            m_codec.Send(m_sp_conn,msg);
        }else{
            LOG_DEBUG(logger)<<"Notify main fun!";
            int id = spRpcMsg->id();
            m_mapResponse[id] = spRpcMsg;
            m_Notify[id].Post();
        }
        

    }

}