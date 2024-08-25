#include "EventLoop.hpp"
#include "Server.hpp"
#include "utils.hpp"
int main()
{
    DEBUG("Server version 20240825_02");
    EventLoop* loop = new EventLoop();
    Server* server = new Server(loop);

    loop->loop();
    return 0;
}