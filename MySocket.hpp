#ifndef SOCKET_HPP__

class InetAddress;

class MySocket
{
private:
    int fd;
public:
    MySocket();
    MySocket(int _fd);
    ~MySocket();

    int accept(InetAddress*);
    void bind(InetAddress*);
    void connect(InetAddress*);
    int getFd();
    void listen();
    void setnonblocking();
};

#endif // !SOCKET_HPP__