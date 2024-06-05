#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUFSIZE 30

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    WSADATA wsaData;
    SOCKET serv_fd, clnt_sd;
    FILE* fp;
    char buf[BUFSIZE];
    int recv_cnt;

    SOCKADDR_IN serv_addr, clnt_addr;
    int clnt_addr_sz;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        ErrorHandling("WSAStartup() error");
    }

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    fp = fopen("file_server_win.c", "rb");
    serv_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_fd == INVALID_SOCKET){
        ErrorHandling("TCP socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_fd, (SOCKADDR*)& serv_addr, sizeof(serv_addr)) == SOCKET_ERROR){
        ErrorHandling("bind() error");
    }

    if(listen(serv_fd, 5) == SOCKET_ERROR){
        ErrorHandling("listen() error");
    }

    clnt_addr_sz = sizeof(clnt_addr);
    clnt_sd = accept(serv_fd, (SOCKADDR*)& clnt_addr, &clnt_addr_sz);
    if(clnt_sd == INVALID_SOCKET){
        ErrorHandling("accept() error");
    }

    while (1)
    {
        recv_cnt = fread((void*)buf, 1, BUFSIZE, fp);
        if(recv_cnt == EOF) ErrorHandling("fread() error");
        if(recv_cnt < BUFSIZE){
            if(send(clnt_sd, (CHAR*)buf, recv_cnt, 0) == SOCKET_ERROR) ErrorHandling("send() error");
            break;
        }
        if(send(clnt_sd, (CHAR*)buf, BUFSIZE, 0) == SOCKET_ERROR) ErrorHandling("send() error");
    }
    
    shutdown(clnt_sd, SD_SEND);
    recv_cnt = recv(clnt_sd, (CHAR*)buf, BUFSIZE - 1, 0);
    if (recv_cnt == SOCKET_ERROR) ErrorHandling("recv() error");
    buf[recv_cnt] = 0;
    printf("Message from client : %s \n", buf);
    fclose(fp);
    closesocket(clnt_sd);
    closesocket(serv_fd);
    WSACleanup();

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}