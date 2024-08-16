#include "Server.hpp"
#include "EventLoop.hpp"
#include "MySocket.hpp"
#include "MyChannel.hpp"
#include "Acceptor.hpp"
#include "utils.hpp"
#include "InetAddress.hpp"
#include <unistd.h>
#include <errno.h>

#define READ_BUFFER_SIZE 1024

Server::Server(EventLoop* _loop): loop(_loop)
{
    acceptor = new Acceptor(loop);
    std::function<void(MySocket*)> cb = std::bind(&Server::connectNewRquest, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
    /*
    MySocket* serv_socket = new MySocket();
    InetAddress* serv_address = new InetAddress("127.0.0.1", 8888);
    serv_socket->bind(serv_address);
    serv_socket->listen();
    serv_socket->setnonblocking();

    MyChannel* server_channel = new MyChannel(loop, serv_socket->getFd());
    server_channel->enableReading();
    std::function<void()> cb = std::bind(&Server::connectNewRquest, this, serv_socket);
    server_channel->setCallback(cb);*/
}

Server::~Server()
{
    delete acceptor;
}

void Server::handleReadEvent(int sockfd)
{
    INFO(__func__);
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
void Server::connectNewRquest(MySocket* serv_socket)
{
    INFO(__func__);
    InetAddress* client_addr = new InetAddress();
    int client_fd = serv_socket->accept(client_addr);
    MySocket* client_socket = new MySocket(client_fd);
    DEBUG("new client fd:", client_fd, " IP:", client_addr->getIP() , " Port:", client_addr->getPort());
    client_socket->setnonblocking();

    MyChannel *client_channel = new MyChannel(loop, client_fd);
    client_channel->enableReading();
    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, client_fd);
    client_channel->setCallback(cb);

    delete client_addr;
    delete client_socket;    // fd in MySocket class will close if delete this, so....
}