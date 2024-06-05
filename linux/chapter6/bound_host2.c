#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in hostaddr_s, hostaddr_d;
    char message1[] = "hello!\n";
    char message2[] = "I'm another host!\n";
    char message3[] = "Nice to meet you!\n";

    if(argc != 3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1){
        ErrorHandling("UDP socket() error");
    }

    memset(&hostaddr_d, 0, sizeof(hostaddr_d));
    hostaddr_d.sin_family = AF_INET;
    hostaddr_d.sin_addr.s_addr = inet_addr(argv[1]);
    hostaddr_d.sin_port = htons(atoi(argv[2]));

    //连续sendto三次，观察接收方主机调用几次recvfrom接收全部数据
    if(sendto(sock, message1, sizeof(message1), 0, (struct sockaddr*)& hostaddr_d, sizeof(hostaddr_d)) == -1){
        ErrorHandling("sento() error");
    }
    if(sendto(sock, message2, sizeof(message2), 0, (struct sockaddr*)& hostaddr_d, sizeof(hostaddr_d)) == -1){
        ErrorHandling("sento() error");
    }
    if(sendto(sock, message3, sizeof(message3), 0, (struct sockaddr*)& hostaddr_d, sizeof(hostaddr_d)) == -1){
        ErrorHandling("sento() error");
    }

    close(sock);

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}