#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>

#define BUFSIZE 50

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    WSADATA wsaData;
    SOCKET serv_sock, io_sock;
    SOCKADDR_IN servaddr, clntaddr;
    fd_set reads, cpy_reads;
    TIMEVAL timeout;

    char buf[BUFSIZE];
    int recv_len, send_len; 
    int fdNum;
    int clntaddr_sz;

    if(argc != 2){
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup() error");

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == INVALID_SOCKET) ErrorHandling("socket() error");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (SOCKADDR*)& servaddr, sizeof(servaddr)) == SOCKET_ERROR) ErrorHandling("bind() error");

    if(listen(serv_sock, 5) == SOCKET_ERROR) ErrorHandling("listen() error");

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    
    while(1){
        cpy_reads = reads;
        timeout.tv_sec  = 5;
        timeout.tv_usec = 0;

        if((fdNum = select(0, &cpy_reads, 0, 0, &timeout)) == -1){
            ErrorHandling("select() error");
            break;
        }
        if(fdNum == 0){
            // puts("Time-out!");
            continue;
        }

        for(int i = 0 ; i < reads.fd_count; ++i){
            if(FD_ISSET(reads.fd_array[i], &cpy_reads)){ //这里不同于Linux，i就代表套接字描述符，这里表示fd_array的第几个元素也即（套接字句柄）
                if(reads.fd_array[i] == serv_sock){
                    clntaddr_sz = sizeof(clntaddr);
                    io_sock = accept(serv_sock, (SOCKADDR*)& clntaddr, &clntaddr_sz);
                    if(io_sock == INVALID_SOCKET) ErrorHandling("accept() error");
                    FD_SET(io_sock, &reads);
                    printf("Connected to Client%d \n", io_sock);
                }
                else{
                    recv_len = recv(reads.fd_array[i], buf, BUFSIZE - 1, 0);
                    if(recv_len == 0) {
                        printf("Client%d closed \n", reads.fd_array[i]);
                        closesocket(reads.fd_array[i]);
                        FD_CLR(reads.fd_array[i], &reads);
                    }
                    else{
                        send_len = send(reads.fd_array[i], buf, recv_len, 0);
                        if(send_len == SOCKET_ERROR) ErrorHandling("send() error"); 
                    }
                }
            }
        }
    }

    closesocket(serv_sock);
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}