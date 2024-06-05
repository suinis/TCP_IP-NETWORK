#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 10 //设置10时为了测试message空间设置小于传过来的message大小的情况

void error_handling(char* message);

int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUFSIZE];
    int str_len = 0, idx = 0;
    ssize_t read_len;

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
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); //第一个参数：要连接的服务端ip地址
    serv_addr.sin_port = htons(atoi(argv[2])); //第二个参数：要连接的服务端端口号

    if(connect(sock, (struct sockaddr*)& serv_addr, sizeof(serv_addr)) == -1){
        error_handling("connect() error");
    }

    for(int i = 0; i < 300; ++i){ //延迟读
        printf("Wait time %d \n", i);
    }
    while ((str_len < BUFSIZE) && (read_len = (read(sock, &message[idx++], 1)))) //当read_len = 0时，说明渠道0个字节，读到缓冲区尾了
    {
        if (read_len == -1)
        {
            error_handling("read() error");
        }
        str_len += read_len;
    }

    printf("Message from server : %s \n", message);
    printf("strlen = %d\n", str_len);
    close(sock);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}