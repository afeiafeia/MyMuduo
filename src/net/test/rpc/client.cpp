#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include "../../Rpc/rpcservice.pb.h"
#include "../../Rpc/rpc.pb.h"
#include "../../Rpc/RpcChannel.h"
#include "../../TcpConnection.h"
#include "../../EventLoop.h"
#include "../../InetAddress.h"
using namespace afa;
int main()
{
    int fd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = 9876;
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);

    if(connect(fd,(struct sockaddr*)(&addr),sizeof(addr))<0)
    {
        std::cout<<"Error in connect"<<std::endl;
    }
    int option = fcntl(fd,F_GETFD);
    int new_option = option|O_NONBLOCK;
    fcntl(fd,F_SETFD,new_option);
    char buff[128] = "Hello World!";
    char read_buff[128];
    sleep(1);
    InetAddress address;
    while(1)
    {
        EventLoop loop;
        SP_TcpConnection sp_conn(new TcpConnection(&loop,fd,address));
        afa::ProtoRequest rpc_rsq;
        rpc_rsq.set_message_name("afei's request");
        
        afa::ProtoResponse* rpc_rsp = new afa::ProtoResponse();
        afa::RpcChannel* rpc_channel = new afa::RpcChannel(sp_conn);
        afa::RpcService_Stub rpc_stub(rpc_channel);
        rpc_stub.GetService(NULL, &rpc_rsq, rpc_rsp,nullptr);
        loop.Loop();
        //int writenum = write(fd,buff,12);
        break;
    }


    return 0;
}
