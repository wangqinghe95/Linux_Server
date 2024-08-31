#include "MyChannel.hpp"
#include "EventLoop.hpp"
#include "utils.hpp"

MyChannel::MyChannel(EventLoop* _loop, int _fd) 
                    : loop(_loop)
                    , fd(_fd)
                    , events(0)
                    , ready(0)
                    , inEpoll(false)
                    , useThreadPool(false)
{
}
MyChannel::~MyChannel()
{
}

void MyChannel::enableRead()
{
    events |= (EPOLLIN | EPOLLPRI);
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
uint32_t MyChannel::getReady()
{
    return ready;
}
bool MyChannel::getInEpoll()
{
    return inEpoll;
}

void MyChannel::handleEvent()
{
    if(ready & (EPOLLIN | EPOLLPRI)) {
        if(useThreadPool) loop->addFunc(readCallback);
        else readCallback();
    }

    if(ready & EPOLLOUT) {
        if(useThreadPool) loop->addFunc(writeCallback);
        else writeCallback();
    }
}

void MyChannel::setReadCallback(std::function<void()> _cb)
{
    readCallback = _cb;
}
void MyChannel::setWriteCallback(std::function<void()> _cb)
{
    writeCallback = _cb;
}

void MyChannel::useET()
{
    events |= EPOLLET;
    loop->updateChannel(this);
}

void MyChannel::setInEpoll(bool _in)
{
    inEpoll = _in;
}
void MyChannel::setReady(uint32_t _ev)
{
    ready = _ev;
}

void MyChannel::setRevent(uint32_t)
{
    
}

void MyChannel::setUseThreadPool(bool _status)
{
    useThreadPool = _status;
}