#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 1024

void ErrorHandling(const char* message);

void read_childpro(int sig);

int main(int argc, char* argv[]){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    pid_t pid;
    char buf[BUFSIZE];
    ssize_t read_len;
    struct sigaction act;

    if(argc != 2){
        printf("Usage : %s <port> \n", argv[0]);
    }

    act.sa_handler = read_childpro;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) ErrorHandling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)& serv_addr, sizeof(serv_addr)) == -1) ErrorHandling("bind() error");

    if(listen(serv_sock, 5) == -1) ErrorHandling("listen() error");

    int clnt_addr_sz = sizeof(clnt_addr); 
    while (1)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)& clnt_addr, &clnt_addr_sz);
        if(clnt_sock == -1) continue; //没建立连接就继续等待客户端连接
        else puts("new client connected...");

        pid = fork();
        if(pid == -1){ //创建子进程失败
            close(clnt_sock); //应该是因为while循环中有accept持续调用，所以这里要释放连接，后面才能继续accept
            continue; //再次建立acept连接，尝试生成子进程
        }
        if (pid == 0)
        {
            close(serv_sock);
            while((read_len = read(clnt_sock, buf, BUFSIZE - 1)) != 0){
                if(read_len == -1) ErrorHandling("read() error");
                buf[read_len] = 0;
                if(write(clnt_sock, buf, read_len) == -1) ErrorHandling("write() error");
                printf("message from client : %s\n", buf);
            }
            
            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        }else{ //子进程已经复制一份clnt_sock用于同客户端通信，主进程不设置数据交换功能，这里关闭主进程clnt_sock
            close(clnt_sock);
        }
    }
    
    close(serv_sock);
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);   
}

void read_childpro(int sig){
    if(sig == SIGCHLD){
        int status;
        pid_t pid;
        while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
        {
            printf("removed pro id : %d \n", pid);
        }
    }
}