#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

void ErrorHandling(char* message);

int main(int argc, char* argv[]){
    WSADATA wsaData;
    SOCKET hSocket;
    SOCKADDR_IN servAddr;

    char message[30];
    int strLen = 0, read_len, idx = 0;
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

    // strLen = recv(hSocket, message, sizeof(message), 0);
    // if(strLen == -1){
    //     ErrorHandling("recv() error");
    // }
    // printf("Message from server:%s\n", message);
    // strLen = recv(hSocket, message, sizeof(message), 0);
    // if(strLen == -1){
    //     ErrorHandling("recv() error");
    // }
    // printf("Message from server:%s\n", message);

    while(1){
        read_len = recv(hSocket, &message[idx], 1, 0);
        if(read_len == -1){
            ErrorHandling("recv() error");
        }else if(read_len == 0){
            printf("get all message!\n");
            break;
        }else{
            strLen += read_len;
            // printf("%c\n", message[idx]);
        }
        idx++;
    }
    printf("strlen(message) : %d\n", strlen(message));
    printf("Message from server:%s\n", message);


    closesocket(hSocket);
    WSACleanup();

    return 0;
}

void ErrorHandling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}