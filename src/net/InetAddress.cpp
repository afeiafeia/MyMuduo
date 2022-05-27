#include "InetAddress.h"
#include <arpa/inet.h>
#include <string.h>
namespace afa
{
    InetAddress::InetAddress()
    {
        bzero(&m_addr,sizeof(m_addr));
    }

    InetAddress::InetAddress(uint16_t port)
    {
        memset(&m_addr,0,sizeof(m_addr));
        m_addr.sin_family = AF_INET;
        m_addr.sin_addr.s_addr = Endian::HostToNetwork32(INADDR_ANY);
        m_addr.sin_port = Endian::HostToNetwork16(port);
        //m_addr.sin_port = port;
    }

    InetAddress::InetAddress(const struct sockaddr_in &addr)
    :m_addr(addr)
    {

    }
    InetAddress::InetAddress(int famliy,std::string ipaddress,uint16_t port)
    {
        m_addr.sin_family = famliy;
        inet_pton(famliy,ipaddress.c_str(),&m_addr.sin_addr);
        m_addr.sin_port = Endian::HostToNetwork16(port);
    }

    InetAddress::~InetAddress()
    {
       
    }

    std::string InetAddress::StrilizeIpAddress() const
    {
        char ip[32] = {'\0'};
        inet_ntop(m_addr.sin_family,(void*)&m_addr.sin_addr,ip,sizeof(ip));
        return std::string(ip);
    }

    std::string InetAddress::StrilizeFamliy() const
    {
        switch (m_addr.sin_family)
        {
        case AF_INET:
                return std::string("AF_INET");
        case AF_INET6:
                return std::string("AF_INET6");
        default:
                return std::string("UnKnown");
        }
    }



}