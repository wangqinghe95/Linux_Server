#include "Server.hpp"
#include "EventLoop.hpp"
#include "MySocket.hpp"
#include "MyChannel.hpp"
#include "Acceptor.hpp"
#include "utils.hpp"
#include "InetAddress.hpp"
#include "Connection.hpp"
#include <unistd.h>
#include <errno.h>

#define READ_BUFFER_SIZE 1024

Server::Server(EventLoop* _loop): loop(_loop)
{
    acceptor = new Acceptor(loop);
    std::function<void(MySocket*)> cb = std::bind(&Server::connectNewRquest, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server()
{
    delete acceptor;
}

void Server::connectNewRquest(MySocket* _socket)
{
    INFO("socket fd:", _socket->getFd());
    if(-1 != _socket->getFd()) {
        Connection* conn = new Connection(loop, _socket);
        std::function<void(int)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        conn->setDeleteConnectionCallback(cb);
        connections[_socket->getFd()] = conn;
    }
}

void Server::deleteConnection(int sockfd)
{
    if(-1 != sockfd) {
        auto it = connections.find(sockfd);
        if(it != connections.end()) {
            Connection* conn = connections[sockfd];
            connections.erase(sockfd);
            // delete conn;
            close(sockfd);
        }
    }
}