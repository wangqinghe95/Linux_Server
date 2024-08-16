#include "MyChannel.hpp"
#include "EventLoop.hpp"
#include "utils.hpp"

MyChannel::MyChannel(EventLoop* _loop, int _fd) 
                    : loop(_loop)
                    , fd(_fd)
                    , events(0)
                    , revents(0)
                    , inEpoll(false)
{
}
MyChannel::~MyChannel()
{
}

void MyChannel::enableReading()
{
    events = EPOLLIN | EPOLLET;
    loop->updateChannel(this);
}
int MyChannel::getFd()
{
    return fd;
}
uint32_t MyChannel::getEvents()
{
    return events;
}
uint32_t MyChannel::getRevetns()
{
    return revents;
}
bool MyChannel::getInEpoll()
{
    return inEpoll;
}

void MyChannel::handleEvent()
{
    callback();
}

void MyChannel::setCallback(std::function<void()> _cb)
{
    callback = _cb;
}

void MyChannel::setInEpoll()
{
    inEpoll = true;
}
void MyChannel::setRevetns(uint32_t _ev)
{
    revents = _ev;
}