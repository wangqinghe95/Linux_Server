#include "inetaddress.hpp"
#include "utils.hpp"
#include <cstring>
#include <unistd.h>

InetAddress::InetAddress() : addr_len(sizeof(addr))
{
    bzero(&addr, sizeof(addr));
}
InetAddress::InetAddress(const char* ip, uint16_t port) : addr_len(sizeof(addr))
{
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    addr_len = sizeof(addr);
}
InetAddress::~InetAddress()
{
}

char* InetAddress::getIP()
{
    return inet_ntoa(addr.sin_addr);
}
const int InetAddress::getPort()
{
    return ntohs(addr.sin_port);
}

void InetAddress::setAdrr(sockaddr_in _addr, socklen_t _addr_len)
{
    addr = _addr;
    addr_len = _addr_len;
}

sockaddr_in InetAddress::getAddr()
{
    return addr;
}
socklen_t InetAddress::getAddrlen()
{
    return addr_len;
}