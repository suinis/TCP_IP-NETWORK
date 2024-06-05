#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    WSADATA wsaData;
    SOCKET sock;
    SOCKADDR_IN recv_adr;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error");

    if(argc != 3){
        printf("Usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
        ErrorHandling("socket() error");

    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    recv_adr.sin_port = htons(atoi(argv[2])); 
    
    if(connect(sock, (SOCKADDR*)& recv_adr, sizeof(recv_adr)) == SOCKET_ERROR)
        ErrorHandling("connect() error");

    send(sock, "123", strlen("123"), 0);
    send(sock, "4", strlen("4"), MSG_OOB);
    send(sock, "567", strlen("567"), 0);
    send(sock, "890", strlen("890"), MSG_OOB);

    closesocket(sock);
    WSACleanup();

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}