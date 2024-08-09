#include "MySocket.hpp"
// #include "InetAddress.hpp"
#include "MyEpoll.hpp"
#include "utils.hpp"
#include <cstdio>
#include <unistd.h>
#include <errno.h>
#include "MyChannel.hpp"

#define READ_BUFFER_SIZE 1024

void handleReadEvent(int sockfd);

int main()
{
    MySocket* serv_socket = new MySocket();
    InetAddress* serv_address = new InetAddress("127.0.0.1", 8888);
    serv_socket->bind(serv_address);
    serv_socket->listen();
    serv_socket->setnonblocking();

    MyEpoll* server_epoll = new MyEpoll();
    // server_epoll->addFd(serv_socket->getFd(), EPOLLIN | EPOLLET);

    MyChannel* server_channel = new MyChannel(server_epoll, serv_socket->getFd());
    server_channel->enableReading();
    while (true)
    {
        std::vector<MyChannel*> activeChannel = server_epoll->poll();
        int nfd = activeChannel.size();
        for(int i = 0; i < nfd; ++i) {
            int chfd = activeChannel[i]->getFd();
            if(chfd == serv_socket->getFd()) {
                InetAddress* client_addr = new InetAddress();
                int client_fd = serv_socket->accept(client_addr);
                MySocket* client_socket = new MySocket(client_fd);
                printf("new client fd:%d IP:%s Port:%d\n", client_fd, client_addr->getIP(), client_addr->getPort());
                client_socket->setnonblocking();
                // server_epoll->addFd(client_fd, EPOLLIN | EPOLLET);

                MyChannel *client_channel = new MyChannel(server_epoll, client_fd);
                client_channel->enableReading();

                delete client_addr;
                delete client_socket;    // fd in MySocket class will close if delete this, so....
            }
            else if(activeChannel[i]->getEvents() & EPOLLIN) {
                printf("handleReadevents\n");
                handleReadEvent(chfd);
            }
            else {
                printf("something else happened.\n");
            }
        }
    }

    delete serv_address;
    delete serv_socket;
    return 0;    
}

void handleReadEvent(int sockfd)
{
    char buf[READ_BUFFER_SIZE];
    while (true)
    {
        bzero(buf, sizeof(READ_BUFFER_SIZE));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if(read_bytes > 0) {
            printf("message from client fd %d : %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        }
        else if(read_bytes == 0) {
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);
            break;
        }
        else {
            if(read_bytes == -1) {
                if(errno == EINTR) {
                    printf("continue reading\n");
                    continue;
                }
                else if(errno == EAGAIN || errno == EWOULDBLOCK) {
                    printf("finish reading once, and res number : %d\n", errno);
                    break;
                }
                else {
                    printf("errno other something : %d\n", errno);
                }
            }
        }
    }
    
}