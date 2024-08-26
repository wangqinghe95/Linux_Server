#ifndef CONNECTION_HPP__
#define  CONNECTION_HPP__

#include <functional>

class EventLoop;
class MySocket;
class MyChannel;
class Buffer;
class Connection
{
private:
    EventLoop *loop;
    MySocket* sock;
    MyChannel* channel;
    Buffer* read_buff;
    std::function<void(MySocket*)> deleteConnectionCallback;
public:
    Connection(EventLoop *_loop, MySocket* _sock);
    ~Connection();

    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(MySocket*)>);
};



#endif // !CONNECTION_HPP__