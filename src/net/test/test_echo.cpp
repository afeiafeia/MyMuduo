#include <iostream>
#include "../EchoServer.h"
#include "../EventLoop.h"
using namespace afa;
int main()
{
    EventLoop loop;
    EchoServer server(&loop,9876,1);


    server.Start();
    loop.Loop();

    return 0;
}