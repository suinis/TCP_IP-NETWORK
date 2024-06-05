#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h> //struct ip_mreq

#define BUFSIZE 30
#define END_SIGNAL "END_OF_COMMUNICATION" // 定义结束标志

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    WSADATA wsaData;
    SOCKET recvsock;
    char buf[BUFSIZE];
    SOCKADDR_IN recv_adr;
    struct ip_mreq joinAdr;

    int recv_len;

    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error");

    if(argc != 3){
        printf("Usage : %s <GROUPIP> <PORT> \n", argv[0]);
        exit(1);
    }

    recvsock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi(argv[2]));

    if(bind(recvsock, (SOCKADDR*)& recv_adr, sizeof(recv_adr)) == SOCKET_ERROR)
        ErrorHandling("bind() error");

    joinAdr.imr_multiaddr.s_addr = inet_addr(argv[1]);
    joinAdr.imr_interface.s_addr = htonl(INADDR_ANY);

    if(setsockopt(recvsock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (CHAR*)&joinAdr, sizeof(joinAdr)) == SOCKET_ERROR)
        ErrorHandling("setsockopt() error");

    while (1)
    {
        recv_len = recvfrom(recvsock, buf, BUFSIZE - 1, 0, NULL, 0);
        if(recv_len < 0)
            break;
        buf[recv_len] = 0;
        if(!strcmp(buf, END_SIGNAL))
            break;
        fputs(buf, stdout);
    }
    
    closesocket(recvsock);
    WSACleanup();

    return 0;
}


void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}