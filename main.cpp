#include "EventLoop.hpp"
#include "Server.hpp"
#include "utils.hpp"
int main()
{
    DEBUG("Server version 20240827_01");
    EventLoop* loop = new EventLoop();
    Server* server = new Server(loop);

    loop->loop();
    return 0;
}