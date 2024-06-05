#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUFSIZE 1024

// int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
void ErrorHandling(char* message);
// int WSACleanup(void);

int main(int argc, char* argv[]){
    WSADATA wsaData;
    SOCKET hServSock, hClntSock;
    SOCKADDR_IN servAddr, clntAddr;
    int szClntAddr;
    
    if(argc != 2){
        printf("Usage:%s <port>\n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){ //初始化winsock
        ErrorHandling("WSAStartup() error!");
    }

    hServSock = socket(PF_INET, SOCK_STREAM, 0);
    if(hServSock == INVALID_SOCKET){
        ErrorHandling("socket() error");
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    if(bind(hServSock, (SOCKADDR*)& servAddr, sizeof(servAddr)) == SOCKET_ERROR){
        ErrorHandling("bind() error");
    }

    if(listen(hServSock, 5) == SOCKET_ERROR){
        ErrorHandling("listen() error");
    }

    szClntAddr = sizeof(clntAddr);
    for(int i = 0; i < 5; ++i){
        char message[BUFSIZE] = "Hello World";
        hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
        if(hClntSock == INVALID_SOCKET){
            ErrorHandling("accept() error");
        }

        printf("message1 : %s\n\n", message);
        send(hClntSock, message, strlen(message), 0);
        int numBytes = sprintf(message, "%d", i + 1);
        printf("numBytes : %d\n", numBytes);
        printf("sizeof(message) : %ld\n", sizeof(message));
        printf("strlen(message) : %d\n", strlen(message));
        printf("message2 : %s\n", message);
        send(hClntSock, message, numBytes + 1, 0); //numBytes + 1是为了传输'\0'字符出啊结束符，否则客户端不知道如何处理不带结束符的字符串（出现错误：{i+1}ello World）。

        printf("Connected to client%d...\n\n", i + 1);
        printf("---------------------------------------------\n");
        closesocket(hClntSock);
    }
    
    closesocket(hServSock);
    WSACleanup();

    return 0;
}

void ErrorHandling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}