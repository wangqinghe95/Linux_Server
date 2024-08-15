#ifndef SERVER_HPP__

class EventLoop;
class MySocket;

class Server
{
private:
    EventLoop* loop;
public:
    Server(EventLoop* _loop);
    ~Server();

    void connectNewRquest(MySocket* serv_socket);
    void handleReadEvent(int);
};


#endif // !SERVER_HPP__