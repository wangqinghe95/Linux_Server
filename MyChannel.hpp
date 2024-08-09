#ifndef CHANNEL_HPP__

#include <sys/epoll.h>

class MyEpoll;

class MyChannel
{
public:
    MyChannel(MyEpoll* _ep, int _fd);
    ~MyChannel();

    void enableReading();
    int getFd();
    uint32_t getEvents();
    uint32_t getRevetns();
    bool getInEpoll();
    void setInEpoll();
    void setRevetns(uint32_t);
private:
    MyEpoll *ep;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
};

#endif // !CHANNEL_HPP__