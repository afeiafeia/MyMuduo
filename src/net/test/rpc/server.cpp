#include <iostream>
#include "../../Rpc/RpcServer.h"
#include "../../EventLoop.h"

#include "../../Rpc/rpcservice.pb.h"
#include "../../../base/Log.h"
namespace afa
{
    static Logger::Ptr logger = LOG_ROOT();

    class RpcServiceImpl:public RpcService
    {
         void GetService(::google::protobuf::RpcController* controller,
                       const ::afa::ProtoRequest* request,
                       ::afa::ProtoResponse* response,
                       ::google::protobuf::Closure* done)
                       {
                           LOG_INFO(logger)<<"RpcServiceImpl::GetService()";
                           response->set_message_name("ProtoResponse");
                       }
    };

}
using namespace afa;
int main()
{
    EventLoop loop;
    RpcServer server(&loop,9876,1);
    RpcServiceImpl* service = new  RpcServiceImpl();
    server.RegisterService(service);

    server.Start();

    loop.Loop();

    return 0;
}