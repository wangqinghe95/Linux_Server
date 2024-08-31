#include "Connection.hpp"
#include "MySocket.hpp"
#include "MyChannel.hpp"
#include "EventLoop.hpp"
#include "utils.hpp"
#include "Buffer.hpp"
#include <unistd.h>
#include <cstring>
#include <string>

#define READ_BUFFER_SIZE 1024

Connection::Connection(EventLoop *_loop, MySocket* _sock)
                        : loop(_loop)
                        , sock(_sock)
                        , channel(nullptr)
                        , readBuffer(new Buffer())
                        , inBuffer(new std::string())
{
    INFO("fd:", sock->getFd());
    channel = new MyChannel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    channel->setReadCallback(cb);
    channel->useET();
    channel->enableRead();
    channel->setUseThreadPool(true);
}

Connection::~Connection()
{
    delete channel;
    delete sock;
    delete readBuffer;
}

void Connection::echo(int sockfd)
{
    char buf[READ_BUFFER_SIZE];
    while (true)
    {
        bzero(buf, sizeof(READ_BUFFER_SIZE));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if(read_bytes > 0) {
            readBuffer->append(buf, read_bytes);
        }
        else if(read_bytes == 0) {
            DEBUG("EOF, client fd", sockfd, " disconnected\n");
            // close(sockfd);  // socked closed will be removed from epoll tree automatically
            deleteConnectionCallback(sockfd);
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
                    DEBUG("message from client fd : ", sockfd, " message : ", readBuffer->c_str());
                    /* When the Server receive all data from client
                     * it should be a callback function to parase request from client
                     * and to call different logic functions to response client
                    */
                    send(sockfd);
                    readBuffer->clear();
                    // int write_res = write(sockfd, readBuffer->c_str(), readBuffer->size());
                    // if(write_res == -1) ERROR("Write client:", sockfd, " error");
                    break;
                }
                else {
                    INFO("errno other something : ", errno);
                }
            }
        }
    }
}

void Connection::send(int sockfd)
{
    int data_size = readBuffer->size();
    char *buff = new char[data_size];
    strcpy(buff, readBuffer->c_str());
    int write_len = data_size;
    while (write_len > 0)
    {
        ssize_t bytes_write = write(sockfd, buff+data_size-write_len,write_len);
        if(bytes_write == -1  && errno == EAGAIN) break;
        write_len -= bytes_write;
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(int)> _cb)
{
    deleteConnectionCallback = _cb;
}
