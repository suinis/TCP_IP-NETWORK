#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void ErrorHandling(const char*);

int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in recv_adr;

    if(argc != 3){
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        ErrorHandling("socket() error");

    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    recv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)& recv_adr, sizeof(recv_adr)) == -1)
        ErrorHandling("connect() error");

    send(sock, "123", strlen("123"), 0);
    close(sock);

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}