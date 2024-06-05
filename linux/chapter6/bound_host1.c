#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 1024

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in hostaddr_s, hostaddr_d;
    char buf[BUFSIZE];
    socklen_t hostaddr_d_sz;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1){
        ErrorHandling("UDP socket() error");
    }

    memset(&hostaddr_s, 0, sizeof(hostaddr_s));
    hostaddr_s.sin_family = AF_INET;
    hostaddr_s.sin_addr.s_addr = htonl(INADDR_ANY);
    hostaddr_s.sin_port = htons(atoi(argv[1]));

    if(bind(sock, (struct sockaddr*)& hostaddr_s, sizeof(hostaddr_s)) == -1){
        ErrorHandling("bind() error");
    }

    for(int i = 0; i < 3; ++i){
        sleep(5);
        hostaddr_d_sz = sizeof(hostaddr_d);
        ssize_t recv_len = recvfrom(sock, buf, BUFSIZE - 1, 0, (struct sockaddr*)&hostaddr_d, &hostaddr_d_sz);
        if(recv_len == -1){
            ErrorHandling("recvfrom() error");
        }
        buf[recv_len] = 0;
        printf("message%d from hostaddr_d : %s", i + 1, buf);
    }
    close(sock);

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}