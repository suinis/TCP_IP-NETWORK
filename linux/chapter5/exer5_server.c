#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 1024
#define INTSIZE 4

void error_hadling(char* message);

int main(int argc, char* argv[]){
    int serv_sock;
    int clnt_sock;
    int str_from_len, read_len, str_to_len;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[BUFSIZE]; 

    if(argc != 2){
        printf("Usgae : %s <port>\r", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0); //1. socket创建套接字
    if(serv_sock == -1){
        error_hadling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1])); //第一个参数：服务端要开放的端口号

    if(bind(serv_sock, (struct sockaddr*)& serv_addr, sizeof(serv_addr)) == -1){ //2. bind绑定主机ip和端口号
        error_hadling("bind() error");
    }

    if(listen(serv_sock, 5) == -1){ //3. listen监听是否有对应连接请求
        error_hadling("listen() error");
    }

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)& clnt_addr, &clnt_addr_size); //4. 发现有连接，接受连接请求
    if(clnt_sock == -1){
        error_hadling("accept() error");
    }

    while (read(clnt_sock, &str_from_len, INTSIZE) > 0)
    {
        // printf("str_from_len = %d\n", str_from_len);
        if((read_len = read(clnt_sock, message, str_from_len)) == -1){
            error_hadling("read() error");
        }
        message[read_len] = 0;
        printf("client : %s\n", message);

        printf("server (you have to write something to client!) : ");
        // scanf("%s" ,message);
        // message[strlen(message)] = 0;
        fgets(message, BUFSIZE, stdin);
        str_to_len = strlen(message) - 1; //strlen计算message长度，fgets会读取一行包括换行符

        if(write(clnt_sock, &str_to_len, INTSIZE) == -1){
            error_hadling("write() error");
        }
        if(write(clnt_sock, message, str_to_len) == -1){
            error_hadling("write() error");
        }
    }
    
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_hadling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}