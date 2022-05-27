#include<iostream>
#include "../InetAddress.h"
#include "../../base/Log.h"
using namespace afa;
Logger::Ptr logger = LOG_ROOT();
int main()
{
    //InetAddress address(AF_INET,"127.0.0.1",126);
    InetAddress address(126);

    std::cout<<address.StrilizeFamliy()<<std::endl;
    std::cout<<address.StrilizeIpAddress()<<std::endl;
    std::cout<<address.Port()<<std::endl;

    return 0;
}