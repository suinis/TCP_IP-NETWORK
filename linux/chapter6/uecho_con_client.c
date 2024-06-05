#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 1024

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int sock; //都只需要一个socket
    struct sockaddr_in serv_addr, from_addr;
    char message[BUFSIZE];
    int mes_len, recv_len;
    socklen_t from_addr_sz;

    if(argc != 3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1){
        ErrorHandling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if(connect(sock, (struct sockaddr*)& serv_addr, sizeof(serv_addr)) == -1){
        ErrorHandling("connect() error");
    }

    while (1)
    {
        fputs("Insert message(q/Q to quit) : ", stdout);
        fgets(message, BUFSIZE, stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")) break;

        mes_len = strlen(message) - 1; //去掉fgets获取到的'\n'换行符，否则服务端会多打印一行换行
        message[mes_len] = 0;
        // if(sendto(sock, message, mes_len, 0, (struct sockaddr*)& serv_addr, sizeof(serv_addr)) == -1){
        //     ErrorHandling("sendto() error");
        // }
        if(write(sock, message, mes_len) == -1){
            ErrorHandling("write() error");
        }
        // from_addr_sz = sizeof(from_addr);
        // if((recv_len = recvfrom(sock, message, BUFSIZE - 1, 0, (struct sockaddr*)& from_addr, &from_addr_sz)) == -1){
        //     ErrorHandling("recvfrom() error");
        // }
        if((recv_len = read(sock, message, BUFSIZE - 1)) == -1){
            ErrorHandling("read() error");
        }
        message[recv_len] = 0;
        printf("message from server : ");
        fputs(message, stdout);
        fputc('\n', stdout);
    }
    close(sock);

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}