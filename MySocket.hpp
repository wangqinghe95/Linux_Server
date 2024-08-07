#ifndef SOCKET_HPP__

#include "inetaddress.hpp"

// class InetAddress;

class MySocket
{
private:
    int fd;
public:
    MySocket();
    MySocket(int _fd);
    ~MySocket();

    void bind(InetAddress*);
    void listen();
    void setnonblocking();

    int accept(InetAddress*);
    int getFd();
};

#endif // !SOCKET_HPP__