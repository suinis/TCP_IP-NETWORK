#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#define BUFSIZE 1024

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    WSADATA wsaData;
    SOCKET sock;
    SOCKADDR_IN sockaddr;
    char message[BUFSIZE];
    int strLen;

    if(argc != 3){
        printf("Usage: %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        ErrorHandling("WSAStartup() error");
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET){
        ErrorHandling("socket() error");
    }

    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(argv[1]);
    sockaddr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (SOCKADDR*)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR){
        ErrorHandling("connect() error");
    }else{
        printf("connected...");
    }

    while(1){
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUFSIZE, stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")){
            break;
        }
        send(sock, message, strlen(message), 0);
        strLen = recv(sock, message, BUFSIZE -1, 0);
        message[strLen] = 0;
        printf("message from server : %s\n", message);
    }
    closesocket(sock);
    WSACleanup();
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}