#ifndef EVENT_LOOP_HPP__

class MyEpoll;
class MyChannel;

class EventLoop
{
private:
    MyEpoll* ep;
    bool quit;
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(MyChannel* _ch);
};

#endif // !EVENT_LOOP_HPP__

