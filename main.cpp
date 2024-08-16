#include "EventLoop.hpp"
#include "Server.hpp"
#include "utils.hpp"
int main()
{
    EventLoop* loop = new EventLoop();
    Server* server = new Server(loop);

    loop->loop();
    return 0;
}