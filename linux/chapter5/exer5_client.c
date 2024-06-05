#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 1024 
#define INTSIZE 4

void error_handling(char* message);

int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUFSIZE];
    int str_to_len, str_from_len, read_len;

    if(argc != 3){
        printf("Usage:%s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0); //1. socket创建套接字
    if(sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // inet_aton(argv[1], &serv_addr.sin_addr);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); //第一个参数：要连接的服务端ip地址
    serv_addr.sin_port = htons(atoi(argv[2])); //第二个参数：要连接的服务端端口号

    if(connect(sock, (struct sockaddr*)& serv_addr, sizeof(serv_addr)) == -1){
        error_handling("connect() error");
    }

    while(1){
        fputs("you (Q/q to quit) : ", stdout);
        fgets(message, BUFSIZE, stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")) break; //退出

        str_to_len = strlen(message) - 1;
        // printf("str_to_len = %d\n", str_to_len);
        if(write(sock, &str_to_len, INTSIZE) == -1){ //传输4字节int类型：字符串长度
            error_handling("write() error");
        }
        if(write(sock, message, str_to_len) == -1){ //传输字符串
            error_handling("write() error");
        }
        if(read(sock, &str_from_len, INTSIZE) == -1){
            error_handling("read() error");
        }
        if((read_len = read(sock, message, str_from_len)) == -1){
            error_handling("read() error");
        }
        message[read_len] = 0;
        printf("server : %s\n", message);
    }

    close(sock);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}