#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
// #include <arpa/inet.h> //sockaddr_in
#include <netinet/in.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 50 

void ErrorHandling(const char*);
void urg_handler(int);

int acpt_sock, recv_sock;

int main(int argc, char* argv[]){
    
    struct sockaddr_in send_adr, recv_adr;
    socklen_t send_adr_sz;
    int recv_len;
    char buf[BUFSIZE];

    int state;
    struct sigaction act;
    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if(argc != 2){
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(acpt_sock == -1) 
        ErrorHandling("socket() error");

    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(acpt_sock, (struct sockaddr*)& recv_adr, sizeof(recv_adr)) == -1) 
        ErrorHandling("bind() error");
    
    if(listen(acpt_sock, 5) == -1)
        ErrorHandling("listen() error");

    send_adr_sz = sizeof(send_adr);
    recv_sock = accept(acpt_sock, (struct sockaddr*)& send_adr, &send_adr_sz);
    if(recv_sock == -1)
        ErrorHandling("accept() error");

    fcntl(recv_sock, __F_SETOWN, getpid()); //设置在SIGURG信号注册之前，指定当前进程是处理SIGURG信号
    state = sigaction(SIGURG, &act, 0);
    while((recv_len = recv(recv_sock, buf, sizeof(buf) - 1, 0)) != 0){
        if(recv_len == -1)
            continue;
        buf[recv_len] = 0;
        puts(buf);
    }

    // while (1){
    //     recv_len = recv(recv_sock, buf, BUFSIZE - 1, MSG_PEEK|MSG_DONTWAIT);
    //     if(recv_len > 0)
    //         break;
    // }
    // buf[recv_len] = 0;
    // printf("Buffering %d bytes: %s \n", recv_len, buf);
    
    close(recv_sock);
    close(acpt_sock);

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void urg_handler(int sig){
    char buf[BUFSIZE];
    int recv_len;
    recv_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_OOB);
    if(recv_len == -1)
        ErrorHandling("recv() error");
    buf[recv_len] = 0;
    printf("Urgent message: %s \n", buf); 
}