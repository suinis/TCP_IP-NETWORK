#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 1024

void error_handling(char* message);

int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUFSIZE];
    int str_len = 0, read_len, idx = 0;

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

    read_len = read(sock, message, sizeof(message)); //换为strlen(message)不行
    printf("sizeof(message) : %ld\n", sizeof(message));
    // while (1) {
    //     read_len = read(sock, &message[idx], 1);
    //     if (read_len == -1) {
    //         error_handling("read() error");
    //         break;
    //     } else if (read_len == 0) {
    //         printf("Connection closed by server.\n");
    //         break;
    //     }
    //     // printf("%c %d\n", message[idx], idx);
    //     str_len += read_len;
    //     if (message[idx] == '\0') { // 如果收到了空字符，表示消息结束
    //         break;
    //     }
    //     idx++;
    // }
    printf("Message from server : %s", message);

    close(sock);
    return 0;
    
    
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}