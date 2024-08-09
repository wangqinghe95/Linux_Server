#include "MyChannel.hpp"
#include "MyEpoll.hpp"
#include "utils.hpp"

MyChannel::MyChannel(MyEpoll* _ep, int _fd) 
                    : ep(_ep)
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
    DEBUG("MyChannel ");
    events = EPOLLIN | EPOLLET;
    ep->updateChannel(this);
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
void MyChannel::setInEpoll()
{
    inEpoll = true;
}
void MyChannel::setRevetns(uint32_t _ev)
{
    revents = _ev;
}