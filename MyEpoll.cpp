#include "MyEpoll.hpp"
#include "utils.hpp"
#include <unistd.h>
#include "MyChannel.hpp"

#define MAX_EVENTS 1024


MyEpoll::MyEpoll() : epfd(-1), events(nullptr)
{
    epfd = epoll_create1(0);
    errif(epfd == -1, "epfd create error");
    INFO("Epoll fd:%d", epfd);

    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(events));
}

MyEpoll::~MyEpoll()
{
    DEBUG("~MyEpoll");
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

/**
 * manage file source of MySocket, fd, to solve source leak in Server.cpp::connectNewRquest()
 * fd will be created by MySocket()
 *          added into addFd()
 *          needed to delete in deleteFd()
 */
void MyEpoll::deleteFd(int fd)
{
    if(fd == -1) return;
    //int res = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
    close(fd);
}

/*
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
*/
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
    // ERROR(n_fds == -1, "epoll wait error");
    for(int i = 0; i < n_fds; ++i) {
        // active_event.push_back(events[i]);
        MyChannel* ch = (MyChannel*)events[i].data.ptr;
        ch->setRevetns(events[i].events);
        active_event.push_back(ch);
    }

    return active_event;
}


