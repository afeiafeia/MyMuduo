#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include "../Endian.h"
int main()
{
    int fd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = afa::Endian::HostToNetwork16(9876);
    //addr.sin_port = 9876;
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);

    if(connect(fd,(struct sockaddr*)(&addr),sizeof(addr))<0)
    {
        std::cout<<strerror(errno)<<std::endl;
        std::cout<<"Error in connect!!!"<<std::endl;
    }
    int option = fcntl(fd,F_GETFD);
    int new_option = option|O_NONBLOCK;
    fcntl(fd,F_SETFD,new_option);
    char buff[128] = "Hello World!";
    char read_buff[128];
    sleep(1);
    //while(1)
    {
        int writenum = write(fd,buff,12);
        shutdown(fd,SHUT_WR);
        sleep(1);
        int n = read(fd,read_buff,12);
        if(n<0)
        {
            if(errno==EAGAIN||errno==EWOULDBLOCK)
            {
                //continue;
            }
            else
            {
                //std::cout<<"Error in connect"<<std::endl;
                //return 0;
            }
        }
        std::cout<<read_buff<<std::endl;
        //break;
    }
    sleep(10);
    std::cout<<"close关闭套接字"<<std::endl;
    close(fd);
    sleep(10);
    return 0;
}
