#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

void ErrorHandling(const char* message);

void ShowSockBufSize(SOCKET sock);

int main(int argc, char* argv[]){
    WSADATA wsaData;
    SOCKET sock;
    int state;
    int rcv_buf_sz, snd_buf_sz;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup() error");

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET) ErrorHandling("socket() error");
    ShowSockBufSize(sock);

    rcv_buf_sz = 1024 * 3;
    snd_buf_sz = 1024 * 3;
    
    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (CHAR*)&rcv_buf_sz, sizeof(rcv_buf_sz));
    if(state) ErrorHandling("setsockopt() error");

    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (CHAR*)&snd_buf_sz, sizeof(snd_buf_sz));
    if(state) ErrorHandling("setsockopt() error");
    ShowSockBufSize(sock);
    closesocket(sock);
    WSACleanup();

    return 0;
}

void ShowSockBufSize(SOCKET sock){
    int rcv_buf_sz, snd_buf_sz, len;
    int state;

    len = sizeof(rcv_buf_sz);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (CHAR*)&rcv_buf_sz, &len);
    if(state) ErrorHandling("getsockopt() error");
    printf("Input Buf Size : %d\n", rcv_buf_sz);

    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (CHAR*)&snd_buf_sz, &len);
    if(state) ErrorHandling("getsockopt() error");
    printf("Output Buf Size : %d\n", snd_buf_sz);

}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}