#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define BUFSIZE 50

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int acpt_sock, recv_sock;
    struct sockaddr_in send_adr, recv_adr;
    int send_adr_sz;
    int recv_len;
    char buf[BUFSIZE];

    if(argc != 2){
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(acpt_sock == -1)
        ErrorHandling("socket() error");

    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(acpt_sock, (struct sockaddr*)& recv_adr, sizeof(recv_adr)) == -1)
        ErrorHandling("bind() error");
    
    if(listen(acpt_sock, 5) == -1)
        ErrorHandling("listen() error");
    
    send_adr_sz = sizeof(send_adr);
    recv_sock = accept(acpt_sock, (struct sockaddr*)& send_adr, &send_adr_sz);
    if(recv_sock == -1)
        ErrorHandling("accept() error");
    
    while(1){
        recv_len = recv(recv_sock, buf, BUFSIZE - 1, MSG_PEEK|MSG_DONTWAIT);
        if(recv_len > 0)
            break;
    }

    buf[recv_len] = 0;
    printf("Buffering %d bytes: %s \n", recv_len, buf);

    recv_len = recv(recv_sock, buf, BUFSIZE - 1, 0);
    buf[recv_len] = 0;
    printf("Read again: %s \n", buf);

    close(recv_sock);
    close(acpt_sock);

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}