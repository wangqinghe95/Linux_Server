#include "MySocket.hpp"
// #include "InetAddress.hpp"
#include "MyEpoll.hpp"
#include "utils.hpp"
#include <cstdio>
#include <unistd.h>
#include <errno.h>

#define READ_BUFFER_SIZE 1024

void handleReadEvent(int sockfd);

int main()
{
    MySocket* serv_socket = new MySocket();
    InetAddress* serv_address = new InetAddress("127.0.0.1", 8888);
    serv_socket->bind(serv_address);
    serv_socket->listen();
    serv_socket->setnonblocking();

    MyEpoll* my_epoll = new MyEpoll();
    my_epoll->addFd(serv_socket->getFd(), EPOLLIN | EPOLLET);

    while (true)
    {
        printf("epoll_wait\n");
        std::vector<epoll_event> events = my_epoll->poll();
        int nfd = events.size();
        printf("nfd : %d\n", nfd);
        for(int i = 0; i < nfd; ++i) {
            if(events[i].data.fd == serv_socket->getFd()) {
                InetAddress* client_addr = new InetAddress();
                int client_fd = serv_socket->accept(client_addr);
                MySocket* client_socket = new MySocket(client_fd);
                printf("new client fd:%d IP:%s Port:%d\n", client_fd, client_addr->getIP(), client_addr->getPort());
                client_socket->setnonblocking();
                my_epoll->addFd(client_fd, EPOLLIN | EPOLLET);

                delete client_addr;
                delete client_socket;    // delete socket will cancel listening fd of client_socket
                printf("if end\n"); 
            }
            else if(events[i].events & EPOLLIN) {
                printf("handleReadevents\n");
                handleReadEvent(events[i].data.fd);
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