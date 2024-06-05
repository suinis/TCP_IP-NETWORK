#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#include <sys/time.h>
#include <sys/select.h>

#include <sys/epoll.h>

#include <errno.h> //errno
#include <fcntl.h> //fcntl

#define EPOLLSIZE 50
#define BUFSIZE 4 //缓冲区大小设置为4，保证一次读取后缓冲区中还留有数据。用于验证条件触发。

void ErrorHandling(const char* message);
void setnonblockingmode(int fd);

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
    int epfd, event_cnt;

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

    epfd = epoll_create(EPOLLSIZE); //生成的epoll例程描述符编号由OS决定，参数无意义
    ep_events = malloc(sizeof(struct epoll_event) * EPOLLSIZE); //epoll_wait函数的第二个参数（监听事件结构体数组）需要动态分配

    setnonblockingmode(serv_sock);
    event.events = EPOLLIN; 
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);
    
    while (1)
    {
        //epoll_wait 函数只有在发生事件时才会返回。
        //当数据通信套接字设置了边缘触发模式，（设置了EPOLLIN ： 此时epoll例程中只监听了客户端连接事件、还有已经建立连接的数据接收事件）
        //那么只有当有新的客户端连接/数据传入缓冲时，epoll_wait函数才会返回。否则就会一直执行监听。
        //对数据接收事件设置了边缘触发：导致数据只有在第一次传入缓冲中时才会返回。也就是因为BUFSIZE设置过小，而数据没能一次性传输，
        //缓冲中还留有数据时，epoll_wait也不会返回，也就不会继续后面代码的执行，也就不会多次传输第一次没有传输的数据。
        event_cnt = epoll_wait(epfd, ep_events, EPOLLSIZE, -1);
        if(event_cnt == -1){
            puts("epoll() error");
            break;
        }

        puts("return epoll_wait");
        for(int i = 0; i < event_cnt; ++i){
            //说明是客户端的请求连接，serv_sock不进行数据传输（只用于建立连接）
            if(ep_events[i].data.fd == serv_sock){
                clnt_addr_sz = sizeof(clntaddr);
                io_sock = accept(serv_sock, (struct sockaddr*)& clntaddr, &clnt_addr_sz);
                if(io_sock == -1) ErrorHandling("accept() error");
                setnonblockingmode(io_sock); //设置io套接字为非阻塞I/O

                //将之后用于io的套接字注册到ep_events中
                // event.events = EPOLLIN;
                event.events = EPOLLIN | EPOLLET; //EPOLLET(edge triggering)，以边缘触发的方式得到事件通知
                event.data.fd = io_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, io_sock, &event);
                printf("connected client: %d \n", io_sock);
            }else{ //说明不是连接，那在这里就只能是客户端的数据通信
                printf("message from client%d : ", ep_events[i].data.fd);
                while(1){
                    read_len = read(ep_events[i].data.fd, buf, BUFSIZE);
                    if(read_len == 0){ //说明读到EOF，需要断开连接
                        epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);                                                                                                                             
                        close(ep_events[i].data.fd);
                        printf("closed client: %d \n", ep_events[i].data.fd);
                        break;
                    }else if (read_len < 0){
                        if(errno == EAGAIN)
                            break;
                    }else{ //说明是某个客户端连接请求发送数据
                        buf[read_len] = 0;
                        printf("%s", buf);
                        write(ep_events[i].data.fd, buf, read_len);
                    }
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

void setnonblockingmode(int fd){
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag|O_NONBLOCK);
}