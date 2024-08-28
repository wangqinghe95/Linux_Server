#include "EventLoop.hpp"
#include "MyEpoll.hpp"
#include "MyChannel.hpp"
#include "utils.hpp"
#include "ThreadPool.hpp"

EventLoop::EventLoop() : ep(nullptr), quit(false)
{
    ep = new MyEpoll();
    threadpool = new ThreadPool();
}
EventLoop::~EventLoop()
{
    if(ep != nullptr) delete ep;
    if(threadpool != nullptr) delete threadpool;
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

void EventLoop::addFunc(std::function<void()> func)
{
    threadpool->addTask(func);
}