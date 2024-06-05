#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFSIZE 30

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int recv_sock, i = 0;
    char buf[BUFSIZE];
    ssize_t recv_len;
    struct sockaddr_in recv_adr;

    if(argc != 2){
        printf("Usage : %s <PORT> \n", argv[0]);
        exit(1);
    }

    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(recv_sock == -1)
        ErrorHandling("socket() error");

    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(recv_sock, (struct sockaddr*)& recv_adr, sizeof(recv_adr)) == -1)
        ErrorHandling("bind() error");

    while (1)
    {
        recv_len = recvfrom(recv_sock, buf, BUFSIZE - 1, 0, NULL, 0);
        if(recv_len < 0)
            break;
        buf[recv_len] = 0;
        printf("message%d : %s \n", ++i, buf);
    }
    
    close(recv_sock);

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}