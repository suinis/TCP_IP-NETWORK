#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUFSIZE 30

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    WSADATA wsaData;
    SOCKET sd;
    FILE* fp;

    char buf[BUFSIZE];
    int read_cnt;
    SOCKADDR_IN serv_addr;

    if(argc != 3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        ErrorHandling("WSAStartup() error");
    }

    fp = fopen("receive.dat", "wb");
    sd = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0 ,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    connect(sd, (SOCKADDR*)& serv_addr, sizeof(serv_addr));

    while ((read_cnt = recv(sd, buf, BUFSIZE, 0)) > 0)
    {
        fwrite((void*)buf, 1, read_cnt, fp);
    }
    
    puts("Received file data");
    send(sd, "Thank you", 10, 0);
    fclose(fp);
    closesocket(sd);
    WSACleanup();

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}