#ifndef INET_ADDRESS_H
#define INET_ADDRESS_H
#include <netinet/in.h>
#include <string>

#include "Endian.h"
#include "../base/noncopyable.h"
namespace afa
{
    class InetAddress:public noncopyable
    {
        //IP地址和端口是以网络字节序的方式存储
    private:
        sockaddr_in  m_addr;
    public:
        InetAddress();
        InetAddress(uint16_t port);
        InetAddress(const struct sockaddr_in &addr);
        InetAddress(int famliy,std::string ipaddress,uint16_t port);
        ~InetAddress();

        uint16_t Port()const
        {
            return Endian::NetworkToHost16(m_addr.sin_port);
        }

        sa_family_t Sin_Family()const
        {
            m_addr.sin_family;
        }

        uint32_t IpAddress()const
        {
            return Endian::NetworkToHost32(m_addr.sin_addr.s_addr);
        }

        sockaddr_in SockAddr()const
        {
            return m_addr;
        }

        void SetSockAddr(const sockaddr_in &addr)
        {
            m_addr = addr;
        }

        std::string StrilizeIpAddress() const;

        std::string StrilizeFamliy() const;

    };
}

#endif