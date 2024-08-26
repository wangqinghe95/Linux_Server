#include "Connection.hpp"
#include "MySocket.hpp"
#include "MyChannel.hpp"
#include "EventLoop.hpp"
#include "utils.hpp"
#include "Buffer.hpp"
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
    read_buff = new Buffer();
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
            // DEBUG("message from client fd : ", sockfd, " message : ", buf);
            // write(sockfd, buf, sizeof(buf));
            read_buff->append(buf, read_bytes);
        }
        else if(read_bytes == 0) {
            DEBUG("EOF, client fd", sockfd, " disconnected\n");
            close(sockfd);  // socked closed will be removed from epoll tree automatically
            deleteConnectionCallback(sock);
            break;
        }
        else {
            if(read_bytes == -1) {
                if(errno == EINTR) {
                    INFO("continue reading\n");
                    continue;
                }
                else if(errno == EAGAIN || errno == EWOULDBLOCK) {
                    // INFO("finish reading once, and res number : ", errno);
                    DEBUG("message from client fd : ", sockfd, " message : ", read_buff->c_str());
                    /* When the Server receive all data from client
                     * it should be a callback function to parase request from client
                     * and to call different logic functions to response client
                    */
                    
                    int write_res = write(sockfd, read_buff->c_str(), read_buff->size());
                    if(write_res == -1) ERROR("Write client:", sockfd, " error");
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
