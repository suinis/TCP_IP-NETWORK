#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 1024

void error_hadling(char* message);

int main(int argc, char* argv[]){
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size; 

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
    for(int i = 0; i < 5; ++i){
        clnt_sock = accept(serv_sock, (struct sockaddr*)& clnt_addr, &clnt_addr_size); //4. 发现有连接，接受连接请求
        if(clnt_sock == -1){
            error_hadling("accept() error");
        }

        char message[BUFSIZE] = "Hello World";
        write(clnt_sock, message, strlen(message)); //向客户端传入消息,使用strlen而不是sizeof可以不传输末尾'\0'字符串结束符
        int numBytes = sprintf(message, "%d", i + 1); //向字符串中printf，用于将i+1转换为字符串重写message,末尾自动补上'\0'
        printf("numbytes : %d\n", numBytes);
        printf("sizeof(message) : %ld\n", sizeof(message)); //sizeof的值一开始定义时就被确定了，即容量空间大小
        printf("strlen(message) : %ld\n", strlen(message));
        write(clnt_sock, message, numBytes + 1); //多传输一个'\0',表示字符串传输结束
        
        printf("connect to client%d...\n", i + 1);
        close(clnt_sock);
    }
    
    close(serv_sock);
    return 0;
}

void error_hadling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}