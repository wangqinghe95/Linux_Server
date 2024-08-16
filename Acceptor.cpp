#include "Acceptor.hpp"
#include "utils.hpp"
#include "MySocket.hpp"
#include "InetAddress.hpp"
#include "MyChannel.hpp"

Acceptor::Acceptor(EventLoop* _loop) : loop(_loop)
{
    sock = new MySocket();
    addr = new InetAddress("127.0.0.1", 8888);
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();

    acceptChannel = new MyChannel(loop, sock->getFd());

    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();
}
Acceptor::~Acceptor()
{
    delete sock;
    delete addr;
    delete acceptChannel;
}

void Acceptor::acceptConnection()
{
    newConnectionCallback(sock);
}
void Acceptor::setNewConnectionCallback(std::function<void(MySocket*)> _cb)
{
    newConnectionCallback = _cb;
}