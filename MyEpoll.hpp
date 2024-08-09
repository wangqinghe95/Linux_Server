#ifndef MY_EPOLL_HPP

#include <sys/epoll.h>
#include <vector>

class MyChannel;

class MyEpoll
{
private:
    int epfd;
    struct epoll_event* events;
public:
    ~MyEpoll();
    MyEpoll();

    void addFd(int fd, uint32_t op);
    void updateChannel(MyChannel*);
    // std::vector<epoll_event> poll(int timeout = -1);
    std::vector<MyChannel*> poll(int timeout = -1);
};

#endif // !MY_EPOLL_HPP

