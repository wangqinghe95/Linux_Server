#include "MySocket.hpp"
#include "utils.hpp"
#include "inetaddress.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

MySocket::MySocket() : fd(-1)
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) ERROR("socket create error"); 
}
MySocket::MySocket(int _fd) : fd(_fd) 
{
    if(fd == -1) {
        ERROR("socket create error, fd:",fd); 
    }
}
MySocket::~MySocket()
{
    DEBUG("~MySocket");
    // if(fd != -1) {
    //     close(fd);
    //     fd = -1;
    // }
}

void MySocket::bind(InetAddress* _addr)
{
    sockaddr_in addr = _addr->getAddr();
    socklen_t addr_len = _addr->getAddrlen();
    
    /*
     if using "_addr" as parameters of ::bind
     client can't bind server, why ? 
     addr and addr_len are temporay varieties?
    */
    int bind_res = ::bind(fd, (sockaddr*)&addr, addr_len);

    if(bind_res == -1) {
        ERROR("socket bind error,fd",fd);
    }  
}
void MySocket::listen()
{
    int listen_res = ::listen(fd, SOMAXCONN);
    // ERROR(listen_res == -1, "socket listen error");
    if(listen_res == -1) ERROR("socket listen error");   
}
void MySocket::setnonblocking()
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);    
}

int MySocket::accept(InetAddress* _addr)
{
    struct sockaddr_in addr;
    socklen_t len_sock_addr = sizeof(addr);
    bzero(&addr, sizeof(addr));
    int client_fd = ::accept(fd, (sockaddr*)&addr, &len_sock_addr);
    // int client_fd = ::accept(fd, (sockaddr*)&_addr->addr, &_addr->addr_len);
    if(client_fd == -1) ERROR("socket accept error, fd",fd);
    
    _addr->setAdrr(addr, len_sock_addr);
    return client_fd;
}
int MySocket::getFd()
{
    return fd;
}