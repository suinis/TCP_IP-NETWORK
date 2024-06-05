#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUFSIZE 1024

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    WSADATA wsaData;
    SOCKET serv_sock; //都只需要一个socket
    SOCKADDR_IN serv_addr, clnt_addr;
    int clnt_addr_sz;
    char buf[BUFSIZE];
    int recv_len;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
    }

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        ErrorHandling("WSAStartup() error");
    }

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == INVALID_SOCKET){
        ErrorHandling("UDP socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (SOCKADDR*)& serv_addr, sizeof(serv_addr)) == SOCKET_ERROR){
        ErrorHandling("bind() error");
    }

    while (1)
    {
        clnt_addr_sz = sizeof(clnt_addr);
        recv_len = recvfrom(serv_sock, buf, BUFSIZE - 1, 0, (SOCKADDR*)& clnt_addr, &clnt_addr_sz);
        if(recv_len == SOCKET_ERROR){
            ErrorHandling("recvfrom() error");
        }
        buf[recv_len] = 0;
        printf("message from client : %s\n", buf);
        if(sendto(serv_sock, buf, recv_len, 0, (SOCKADDR*)& clnt_addr, clnt_addr_sz) == SOCKET_ERROR){
            ErrorHandling("sento() error");
        }
    }
    
    closesocket(serv_sock);
    WSACleanup();
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}