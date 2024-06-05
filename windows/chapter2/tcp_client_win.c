#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

void ErrorHandling(char* message);

int main(int argc, char* argv[]){
    WSADATA wsaData;
    SOCKET hSocket;
    SOCKADDR_IN servAddr;

    char message[30];
    int strLen = 0, idx = 0;
    int read_len;

    if(argc != 3){
        printf("Usage:%s <IP> <port>\n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        ErrorHandling("WSAStartup() error");
    }

    hSocket = socket(PF_INET, SOCK_STREAM, 0);
    if(hSocket == INVALID_SOCKET){
        ErrorHandling("socket() error");
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));

    if(connect(hSocket, (SOCKADDR*)& servAddr, sizeof(servAddr)) == SOCKET_ERROR){
        ErrorHandling("connect() error");
    }

    while(read_len = recv(hSocket, &message[idx++], 1, 0)){
        if(read_len == -1){
            ErrorHandling("recv() error");
        }
        strLen += read_len;
    }

    printf("Message from server:%s\n", message);
    printf("strLen = %d", strLen);

    closesocket(hSocket);
    WSACleanup();

    return 0;
}

void ErrorHandling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}