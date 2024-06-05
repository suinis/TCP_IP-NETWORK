#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <string.h>

#define BUFSIZE 1024

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    WSADATA wsaData;
    SOCKET servsock, clntsock;
    SOCKADDR_IN serv_addr, clnt_addr;
    int clnt_addr_sz;
    char buf[BUFSIZE];
    int i, strLen;

    if(argc != 2){
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        ErrorHandling("WSAStartup() error");
    }

    servsock = socket(PF_INET, SOCK_STREAM, 0);
    if(servsock == INVALID_SOCKET){
        ErrorHandling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(servsock, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR){
        ErrorHandling("bind() error");
    }

    if(listen(servsock, 5) == SOCKET_ERROR){
        ErrorHandling("listen() error");
    }

    clnt_addr_sz = sizeof(clnt_addr);
    for(i = 0; i < 5; ++i){
        clntsock = accept(servsock, (SOCKADDR*)&clnt_addr, &clnt_addr_sz);
        if(clntsock == INVALID_SOCKET){
            ErrorHandling("accept() error");
        }else{
            printf("connect client %d\n", i + 1);
        }
        while((strLen = recv(clntsock, buf, BUFSIZE, 0)) != 0){
            send(clntsock, buf, strLen, 0);
        }
        
        closesocket(clntsock);
    }

    closesocket(servsock);
    WSACleanup();
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}