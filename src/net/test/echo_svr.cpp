#include "../EchoServer.h"
#include "../EventLoop.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <unistd.h>
using namespace afa;

void test()
{
    std::cout<<"This is a delay function"<<std::endl;
}
int main()
{
    EventLoop loop;
    EchoServer server(&loop,9876,0);

    server.Start();

    loop.RunAfter(test,3000);
    sleep(1);
    loop.Loop();

    return 0;
}