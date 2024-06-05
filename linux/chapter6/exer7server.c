#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 1024

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int serv_sock; //都只需要一个socket
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;
    char buf[BUFSIZE];
    int recv_len, write_len;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1){
        ErrorHandling("UDP socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)& serv_addr, sizeof(serv_addr)) == -1){
        ErrorHandling("bind() error");
    }

    while (1)
    {
        clnt_addr_sz = sizeof(clnt_addr);
        recv_len = recvfrom(serv_sock, buf, BUFSIZE - 1, 0, (struct sockaddr*)& clnt_addr, &clnt_addr_sz);
        if(recv_len == -1){
            ErrorHandling("recvfrom() error");
        }
        buf[recv_len] = 0;
        printf("message from client : %s\n", buf);

        fputs("Insert message(q/Q to quit) : ",stdout);
        fgets(buf, BUFSIZE, stdin);
        write_len = strlen(buf) - 1; //去掉'\n'
        buf[write_len] = 0;
        if(sendto(serv_sock, buf, write_len, 0, (struct sockaddr*)& clnt_addr, clnt_addr_sz) == -1){
            ErrorHandling("sento() error");
        }
    }
    
    close(serv_sock);
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}