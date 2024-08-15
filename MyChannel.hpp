#ifndef CHANNEL_HPP__

#include <sys/epoll.h>
#include <functional>

class EventLoop;

class MyChannel
{
public:
    MyChannel(EventLoop* _loop, int _fd);
    ~MyChannel();

    void enableReading();

    uint32_t getEvents();
    int getFd();
    bool getInEpoll();
    uint32_t getRevetns();

    void handleEvent();

    void setCallback(std::function<void()>);
    void setInEpoll();
    void setRevetns(uint32_t);
private:
    EventLoop *loop;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
    std::function<void()> callback;
};

#endif // !CHANNEL_HPP__