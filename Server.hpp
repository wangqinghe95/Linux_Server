#ifndef SERVER_HPP__

#include <map>

class EventLoop;
class MySocket;
class Acceptor;
class Connection;

class Server
{
private:
    EventLoop* loop;
    Acceptor* acceptor;
    std::map<int, Connection*> connections;
public:
    Server(EventLoop* _loop);
    ~Server();

    void connectNewRquest(MySocket*);
    void deleteConnection(int sockfd);
};


#endif // !SERVER_HPP__