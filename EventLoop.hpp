#ifndef EVENT_LOOP_HPP__
#define EVENT_LOOP_HPP__
#include <functional>
class MyEpoll;
class MyChannel;
class ThreadPool;

class EventLoop
{
private:
    MyEpoll* ep;
    ThreadPool* threadpool;
    bool quit;
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(MyChannel* _ch);
    void addFunc(std::function<void()>);
};

#endif // !EVENT_LOOP_HPP__

