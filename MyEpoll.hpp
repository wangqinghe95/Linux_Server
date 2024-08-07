#ifndef MY_EPOLL_HPP

#include <sys/epoll.h>
#include <vector>

class MyEpoll
{
private:
    int epfd;
    struct epoll_event* events;
public:
    ~MyEpoll();
    MyEpoll();

    void addFd(int fd, uint32_t op);
    std::vector<epoll_event> poll(int timeout = -1);
};

#endif // !MY_EPOLL_HPP

