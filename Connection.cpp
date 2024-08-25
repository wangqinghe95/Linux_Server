#include "Connection.hpp"
#include "MySocket.hpp"
#include "MyChannel.hpp"
#include "EventLoop.hpp"
#include "utils.hpp"
#include <unistd.h>
#include <cstring>

#define READ_BUFFER_SIZE 1024

Connection::Connection(EventLoop *_loop, MySocket* _sock)
                        : loop(_loop), sock(_sock), channel(nullptr)
{
    channel = new MyChannel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    channel->setCallback(cb);
    channel->enableReading(); 
}

Connection::~Connection()
{
    delete channel;
    delete sock;
}

void Connection::echo(int sockfd)
{
    char buf[READ_BUFFER_SIZE];
    while (true)
    {
        bzero(buf, sizeof(READ_BUFFER_SIZE));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if(read_bytes > 0) {
            DEBUG("message from client fd : ", sockfd, " message : ", buf);
            write(sockfd, buf, sizeof(buf));
        }
        else if(read_bytes == 0) {
            INFO("EOF, client fd", sockfd, " disconnected\n");
            close(sockfd);  // socked closed will be removed from epoll tree automatically
            break;
        }
        else {
            if(read_bytes == -1) {
                if(errno == EINTR) {
                    INFO("continue reading\n");
                    continue;
                }
                else if(errno == EAGAIN || errno == EWOULDBLOCK) {
                    INFO("finish reading once, and res number : ", errno);
                    break;
                }
                else {
                    INFO("errno other something : ", errno);
                }
            }
        }
    }
}
void Connection::setDeleteConnectionCallback(std::function<void(MySocket*)> _cb)
{
    deleteConnectionCallback = _cb;
}
