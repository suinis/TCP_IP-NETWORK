#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#include <sys/time.h>
#include <sys/select.h>

#include <sys/epoll.h>

#define EPOLLSIZE 50
#define BUFSIZE 100

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int serv_sock, io_sock;
    struct sockaddr_in servaddr;
    struct sockaddr_in clntaddr;
    socklen_t clnt_addr_sz;
    char buf[BUFSIZE];
    ssize_t read_len; 
    int i;

    // FILE* readfp;
    // FILE* writefp;
    struct epoll_event* ep_events;
    struct epoll_event event;
    int epfd, epoll_cnt;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    if((serv_sock = socket(PF_INET, SOCK_STREAM, 0))  == -1){
        ErrorHandling("socket() error");
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&servaddr, sizeof(servaddr))  == -1){
        ErrorHandling("bind() error");
    }
    
    if(listen(serv_sock, 5) == -1){
        ErrorHandling("listen() error");
    }

    epfd = epoll_create(EPOLLSIZE);
    ep_events = malloc(sizeof(struct epoll_event) * EPOLLSIZE); //epoll_wait函数的需要动态分配
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);
    
    while (1)
    {
        epoll_cnt = epoll_wait(epfd, ep_events, EPOLLSIZE, -1);
        if(epoll_cnt == -1){
            puts("epoll() error");
            break;
        }

        for(int i = 0; i < epoll_cnt; ++i){
            //说明是客户端的请求连接，serv_sock不进行数据传输（只用于建立连接）
            if(ep_events[i].data.fd == serv_sock){
                clnt_addr_sz = sizeof(clntaddr);
                io_sock = accept(serv_sock, (struct sockaddr*)& clntaddr, &clnt_addr_sz);
                if(io_sock == -1) ErrorHandling("accept() error");

                //将之后用于io的套接字注册到ep_events中
                event.events = EPOLLIN;
                event.data.fd = io_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, io_sock, &event);
                printf("connected client: %d \n", io_sock);
            }else{ //说明不是连接，那在这里就只能是客户端的数据通信
                read_len = read(ep_events[i].data.fd, buf, BUFSIZE);
                if(read_len == 0){ //说明读到EOF，需要断开连接
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                    printf("closed client: %d \n", ep_events[i].data.fd);
                }else{ //说明是某个客户端连接请求发送数据
                    buf[read_len] = 0;
                    printf("message from client%d : %s \n", ep_events[i].data.fd, buf);
                    write(ep_events[i].data.fd, buf, read_len);
                }
            }
        }
    }
    
    close(serv_sock);
    close(epfd);
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}