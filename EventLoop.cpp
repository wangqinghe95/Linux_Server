#include "EventLoop.hpp"
#include "MyEpoll.hpp"
#include "MyChannel.hpp"

EventLoop::EventLoop() : ep(nullptr), quit(false)
{
    ep = new MyEpoll();
}
EventLoop::~EventLoop()
{
    if(ep != nullptr) delete ep;
}

void EventLoop::loop()
{
    while (!quit)
    {
        std::vector<MyChannel*> chs;
        chs = ep->poll();
        for(auto it = chs.begin(); it != chs.end(); ++it) {
            (*it)->handleEvent();
        }
    }
    
}
void EventLoop::updateChannel(MyChannel* _ch)
{
    ep->updateChannel(_ch);
}