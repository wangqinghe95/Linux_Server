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

    void deleteChannel(MyChannel*);
    void updateChannel(MyChannel*);
    std::vector<MyChannel*> poll(int timeout = -1);
};

#endif // !MY_EPOLL_HPP

