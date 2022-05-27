#include "../http/HttpServer.h"
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
    HttpServer server(&loop,9876);

    server.Start();

    loop.Loop();

    return 0;
}