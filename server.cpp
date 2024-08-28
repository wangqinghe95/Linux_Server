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

void Server::connectNewRquest(MySocket* serv_socket)
{
    INFO("socket fd:", serv_socket->getFd());
    Connection* conn = new Connection(loop, serv_socket);
    std::function<void(MySocket*)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections[serv_socket->getFd()] = conn;
}

void Server::deleteConnection(MySocket* sock)
{
    Connection* conn = connections[sock->getFd()];
    connections.erase(sock->getFd());
    delete conn;
}