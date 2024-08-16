#ifndef ACCEPTOR_HPP__

#include <functional>

class EventLoop;
class MySocket;
class InetAddress;
class MyChannel;

class Acceptor
{
private:
    EventLoop* loop;
    MySocket* sock;
    InetAddress* addr;
    MyChannel* acceptChannel;
public:
    Acceptor(EventLoop* _loop);
    ~Acceptor();

    void acceptConnection();
    void setNewConnectionCallback(std::function<void(MySocket*)>);

    std::function<void(MySocket*)> newConnectionCallback;
};

#endif // !ACCEPTOR_HPP__