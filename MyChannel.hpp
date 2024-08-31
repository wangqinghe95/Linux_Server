#ifndef CHANNEL_HPP__

#include <sys/epoll.h>
#include <functional>

class EventLoop;

class MyChannel
{
public:
    MyChannel(EventLoop* _loop, int _fd);
    ~MyChannel();

    void enableRead();

    uint32_t getEvents();
    int getFd();
    bool getInEpoll();
    uint32_t getReady();

    void handleEvent();

    void useET();
    void setInEpoll(bool _in = true);
    void setReadCallback(std::function<void()>);
    void setWriteCallback(std::function<void()>);
    void setReady(uint32_t);
    void setUseThreadPool(bool);
    void setRevent(uint32_t);
private:
    EventLoop *loop;
    int fd;
    uint32_t events;
    uint32_t ready;
    bool inEpoll;
    bool useThreadPool;
    std::function<void()> readCallback;
    std::function<void()> writeCallback;
};

#endif // !CHANNEL_HPP__