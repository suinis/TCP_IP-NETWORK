#define _XOPEN_SOURCE

#include <stdio.h> //fflush
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
    int fds[2];
    pid_t pid;
    char buf[BUFSIZE];
    ssize_t read_len;
    struct sigaction act;

    if(argc != 2){
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
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

    pipe(fds);
    pid = fork();
    if(pid == 0){
        // 子进程代码
        FILE* fp = fopen("echomsg.txt", "w"); //写入文本文件，创建新文件/清空现有文件;
        if(fp == NULL) ErrorHandling("fopen() error");
        char msgbuf[BUFSIZE];
        ssize_t len;
        size_t write_len;
        
        for(int i = 0; i < 10; ++i){
            len = read(fds[0], msgbuf, BUFSIZE - 2);
            msgbuf[len] = 0;
            // msgbuf[len] = '\n';
            // msgbuf[len + 1] = 0;
            write_len = fprintf(fp, "%s\n", msgbuf);  // 使用fprintf来写入，同时加上换行符
            printf("get message from pipe : %s, len : %ld\n", msgbuf, len);
            // write_len = fwrite(msgbuf, 1, len + 1, fp); //将换行符也写入
            printf("write_len : %ld\n", write_len);

            //fflush 是一个用于刷新流缓冲区的函数。在C语言中，文件通常会被缓冲，这意味着写入文件的数据首先被存储在内存中的缓冲区中，然后根据需要再写入到实际的文件中。fflush 函数用于强制将缓冲区中的数据立即写入到文件中，这样可以确保数据被及时地写入磁盘，而不是等到缓冲区满或者关闭文件时才写入。
            fflush(fp);  //这里不设置刷新缓冲，若客户端未传入满10条消息，则不会执行完for循环，此时直接ctrl + c关闭主进程，会导致该子进程执行不到fclose。也就不会执行关闭文件时的将缓冲区中数据写入文件动作。
        }
        fclose(fp);
        return 0; //不返回还会继续往下执行父进程代码
    }

    while (1)
    {
        int clnt_addr_sz = sizeof(clnt_addr); 
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

                write(fds[1], buf, read_len);
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