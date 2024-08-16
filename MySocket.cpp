#include "MySocket.hpp"
#include "utils.hpp"
#include "inetaddress.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

MySocket::MySocket() : fd(-1)
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR(fd == -1, "socket create error");
}
MySocket::MySocket(int _fd) : fd(_fd) 
{
    ERROR(fd == -1, "socket create error");   
}
MySocket::~MySocket()
{
    // if(fd != -1) {
    //     close(fd);
    //     fd = -1;
    // }
}

void MySocket::bind(InetAddress* addr)
{
    int bind_res = ::bind(fd, (sockaddr*)&addr->addr, addr->addr_len);
    ERROR(bind_res == -1, "socket bind error");   
}
void MySocket::listen()
{
    int listen_res = ::listen(fd, SOMAXCONN);
    ERROR(listen_res == -1, "socket listen error");    
}
void MySocket::setnonblocking()
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);    
}

int MySocket::accept(InetAddress* addr)
{
    int client_fd = ::accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    ERROR(client_fd == -1, "socket accept error");
    
    // printf("new client fd %d! IP: %s Port:%d\n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    return client_fd;
}
int MySocket::getFd()
{
    return fd;
}