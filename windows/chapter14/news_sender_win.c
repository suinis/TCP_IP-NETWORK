#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h> //IP_MULTICAST_TTL

#define BUFSIZE 30
#define TTL 64
#define END_SIGNAL "END_OF_COMMUNICATION" // 定义结束标志

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int sendsock;
    char buf[BUFSIZE];
    SOCKADDR_IN mul_adr;
    WSADATA wsaData;
    FILE* fp;

    int time_live = TTL;
    int fread_len;

    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error");

    if(argc != 3){
        printf("Usage : %s <GROUPIP> <PORT> \n", argv[0]);
        exit(1);
    }

    sendsock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sendsock == INVALID_SOCKET)
        ErrorHandling("socket() error");

    memset(&mul_adr, 0, sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]);
    mul_adr.sin_port = htons(atoi(argv[2]));

    if(setsockopt(sendsock, IPPROTO_IP, IP_MULTICAST_TTL, (CHAR*)&time_live, sizeof(time_live)) == SOCKET_ERROR)
        ErrorHandling("setsockopt() error");

    if((fp = fopen("news.txt", "r")) == NULL)
        ErrorHandling("fopen() error");

    while (!feof(fp))
    {
        fgets(buf, BUFSIZE, fp);
        printf("message : %s \n", buf);
        sendto(sendsock, buf, strlen(buf), 0, (SOCKADDR*)& mul_adr, sizeof(mul_adr));
        Sleep(2);
    }
    
    // 发送结束信号
    sendto(sendsock, END_SIGNAL, strlen(END_SIGNAL), 0, (SOCKADDR*)&mul_adr, sizeof(mul_adr));

    fclose(fp);
    closesocket(sendsock);
    WSACleanup();

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}