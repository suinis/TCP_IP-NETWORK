#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#define BUFSIZE 1024

void ErrorHandling(const char* message);
void read_routine(int sock, char* buf);
void write_routine(int sock, char* buf);

int main(int argc, char* argv[]){
    int clnt_sock;
    struct sockaddr_in serv_addr;
    char buf[BUFSIZE];
    pid_t pid;

    if(argc != 3){
        printf("Usage : %s <IP> <port> \n", argv[0]);
    }

    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(clnt_sock == -1){
        ErrorHandling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        ErrorHandling("connect() error");
    }

    pid = fork();
    if(pid == 0){ //子进程控制数据发送
        write_routine(clnt_sock, buf);
    }else{ //主进程控制数据接收
        read_routine(clnt_sock, buf);
    }
    
    close(clnt_sock);
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void read_routine(int sock, char* buf){
    ssize_t read_len;
    while (1)
    {
        read_len = read(sock, buf, BUFSIZE-1);
        if(read_len == 0) return;
        if(read_len == -1) ErrorHandling("read() error");
        buf[read_len] = 0; // (char)0 -> '\0' 
        // printf("message from server : %s\n", buf);
    }
}

void write_routine(int sock, char* buf){
    ssize_t write_len;
    while (1)
    {
        // fputs("Input message(Q to quit): \n", stdout);
        fgets(buf, BUFSIZE, stdin);
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")){
            shutdown(sock, SHUT_WR); //半关闭，只关闭输出流，会明确向服务器端发送EOF
            // close(sock); //使用close
            break;
        }
        write_len = write(sock, buf, strlen(buf) - 1); //去掉'\n'换行符
        if(write_len == -1) ErrorHandling("write() error");
    }
}