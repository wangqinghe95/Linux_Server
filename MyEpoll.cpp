#include "myepoll.hpp"
#include "utils.hpp"
#include <unistd.h>

#define MAX_EVENTS 1024


MyEpoll::MyEpoll() : epfd(-1), events(nullptr)
{
    epfd = epoll_create1(0);
    errif(epfd == -1, "epfd create error");

    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(events));
}

MyEpoll::~MyEpoll()
{
    if(epfd != -1) close(epfd);
    delete [] events;
}


void MyEpoll::addFd(int fd, uint32_t op)
{
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET;
    // setnonblocking(sockfd);
    int res = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    errif(res == -1, "epoll add event error");
}
std::vector<epoll_event> MyEpoll::poll(int timeout)
{
    std::vector<epoll_event> active_event;
    int n_fds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errif(n_fds == -1, "epoll wait error");
    for(int i = 0; i < n_fds; ++i) {
        active_event.push_back(events[i]);
    }

    return active_event;
}