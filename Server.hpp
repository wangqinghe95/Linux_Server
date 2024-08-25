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

    void connectNewRquest(MySocket* serv_socket);
    void deleteConnection(MySocket* sock);
    void handleReadEvent(int);
};


#endif // !SERVER_HPP__