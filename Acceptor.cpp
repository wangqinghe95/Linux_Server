#include "Acceptor.hpp"
#include "utils.hpp"
#include "MySocket.hpp"
#include "InetAddress.hpp"
#include "MyChannel.hpp"

Acceptor::Acceptor(EventLoop* _loop) : loop(_loop)
{
    sock = new MySocket();
    InetAddress* addr = new InetAddress("127.0.0.1", 8888);
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();

    acceptChannel = new MyChannel(loop, sock->getFd());

    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();

    delete addr;
}
Acceptor::~Acceptor()
{
    delete sock;
    delete acceptChannel;
}

void Acceptor::acceptConnection()
{
    InetAddress* client_address = new InetAddress();
    
    MySocket* client_socket = new MySocket(sock->accept(client_address));
    
    DEBUG("New client fd:",client_socket->getFd(), ", IP:", client_address->getIP(), ",Port:", client_address->getPort());
    client_socket->setnonblocking();
    newConnectionCallback(client_socket);

    delete client_address;

}
void Acceptor::setNewConnectionCallback(std::function<void(MySocket*)> _cb)
{
    newConnectionCallback = _cb;
}