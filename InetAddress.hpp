#ifndef INET_ADDRESS_HPP__

#include <arpa/inet.h>

class InetAddress
{
public:
    struct sockaddr_in addr;
    socklen_t addr_len;
public:
    InetAddress();
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress();

    char* getIP();
    const int getPort();
};

#endif // !INET_ADDRESS_HPP__

