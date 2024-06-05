#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

// int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
void ErrorHandling(char* message);
// int WSACleanup(void);

int main(int argc, char* argv[]){
    WSADATA wsaData;
    SOCKET hServSock, hClntSock;
    SOCKADDR_IN servAddr, clntAddr;
    int szClntAddr;
    int idx = 0, write_len;
    char message[400];
    for(int i = 0; i < 400; ++i){
        message[i] = 'c';
    }
    // char message[] = "Hello World!";
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
    hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
    if(hClntSock == INVALID_SOCKET){
        ErrorHandling("accept() error");
    }

    while((idx < sizeof(message)) && (write_len = send(hClntSock, &message[idx++], 1, 0))){
        if(write_len == -1){
            ErrorHandling("write() error");
        }
        printf("have input %d char, sizoef(message) = %d\n", idx, sizeof(message));
    }
    
    
    closesocket(hServSock);
    closesocket(hClntSock);
    WSACleanup();

    return 0;
}

void ErrorHandling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}