#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_EVENTS 1024
#define READ_BUFF_LEN 1024

void errif(bool condition, const char* errmsg)
{
    if(condition) {
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}

void setnonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main()
{
    // 建立一个 socket 套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    // 套接字绑定的服务器 ip 地址
    int bind_res = bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    errif(bind_res == -1, "socket bind error");

    // 服务器监听客户端请求
    int listen_res = listen(sockfd, SOMAXCONN);
    errif(listen_res == -1, "socket listen error");
/*
    // 构造请求的客户端 ip 地址信息
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    bzero(&client_addr, sizeof(client_addr));

    // 同意与客户端连接
    int client_fd = accept(sockfd, (sockaddr*)&client_addr, &client_len);
    errif(client_fd == -1, "socket accept error");
    
    printf("new client fd %d! IP: %s Port:%d\n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
*/
    int epfd = epoll_create(10);
    errif(epfd < 0, "epfd create error");

    struct epoll_event events[MAX_EVENTS], ev;
    bzero(&events, sizeof(events));

    bzero(&ev, sizeof(ev));
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;
    
    setnonblocking(sockfd);
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

    while (true)
    {
        int n_fds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        errif(n_fds == -1, "epoll wait error");
        for(int i = 0; i < n_fds; ++i) {
            if(events[i].data.fd == sockfd) {
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                bzero(&client_addr, sizeof(client_addr));
                
                int client_fd = accept(sockfd, (sockaddr*)&client_addr, &client_len);
                errif(client_fd == -1, "socket accept error");
    
                printf("new client fd %d! IP: %s Port:%d\n", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                bzero(&ev, sizeof(ev));
                ev.data.fd = client_fd;
                ev.events = EPOLLIN | EPOLLET;
                setnonblocking(client_fd);
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);
            }
            else if(events[i].events & EPOLLIN) {
                char buff[READ_BUFF_LEN] = {0};
                    // 与客户端通信，循环读取客户端的消息
                while (true)
                {
                    char buff[1024] = {0};
                    bzero(&buff, sizeof(buff));
                    int request_fd = events[i].data.fd;
                    ssize_t read_bytes = read(request_fd, buff, sizeof(buff));
                    if(read_bytes > 0) {
                        printf("message from client fd : %d : %s\n", request_fd, buff);
                        write(request_fd, buff, sizeof(buff));   // 将客户端发送回来的数据再写回去
                    }
                    else if(read_bytes < 0) {
                        if(errno == EINTR) {
                            printf("continue reading...\n");
                            continue;
                        }
                        else if((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                            printf("finish reading message, errno : %d\n", errno);
                            break;
                        }
                        else {
                            printf("other error\n");
                            break;
                        }
                    }
                    else {
                        printf("client fd : %d disconnected\n", request_fd);
                        close(request_fd);
                        break;
                    }
                }
            }
        }
    }

    close(sockfd);
    return 0;
}