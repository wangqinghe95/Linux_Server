#include "MyEpoll.hpp"
#include "utils.hpp"
#include <unistd.h>
#include "MyChannel.hpp"

#define MAX_EVENTS 1024


MyEpoll::MyEpoll() : epfd(-1), events(nullptr)
{
    epfd = epoll_create1(0);
    if(epfd == -1) ERROR("Epoll fd:%d", epfd);

    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(events));
}

MyEpoll::~MyEpoll()
{
    DEBUG("~MyEpoll");
    if(epfd != -1) close(epfd);
    delete [] events;
}

void MyEpoll::deleteChannel(MyChannel* _channel)
{
    int fd = _channel->getFd();
    int res = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
    if(-1 == res) ERROR("epoll delete error, epfd:", epfd);
    _channel->setInEpoll(false);
}

void MyEpoll::updateChannel(MyChannel* channel)
{
    INFO(__func__);
    int fd = channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));

    ev.data.ptr = channel;
    ev.events = channel->getEvents();
    if(!channel->getInEpoll()) {
        // errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
        int res = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
        if(res == -1) {
            INFO("epoll add error, fd:%s epfd:%d", fd, epfd);
            exit(EXIT_FAILURE);
        }
        channel->setInEpoll();
    }
    else {
        errif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
    }
}

std::vector<MyChannel*> MyEpoll::poll(int timeout)
{
    std::vector<MyChannel*> active_event;
    DEBUG("Wait client connect or send message...");
    int n_fds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    if(n_fds == -1) ERROR("epoll wait error, n_fds:", n_fds);
    else DEBUG("receive client request:", epfd, " and nfd_s:", n_fds);
    // ERROR(n_fds == -1, "epoll wait error");
    for(int i = 0; i < n_fds; ++i) {
        // active_event.push_back(events[i]);
        MyChannel* ch = (MyChannel*)events[i].data.ptr;
        ch->setReady(events[i].events);
        active_event.push_back(ch);
    }

    return active_event;
}


