#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

void ErrorHandling(const char* message);

void ShowSockBufSize(int sock);

int main(int argc, char* argv[]){
    int sock;
    int snd_buf = 1024 * 3, rcv_buf = 1024 * 3;
    int state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) ErrorHandling("socket() error");
    ShowSockBufSize(sock);

    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, sizeof(rcv_buf));
    if(state) ErrorHandling("setsockopt() error");
    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, sizeof(snd_buf));
    if(state) ErrorHandling("setsockopt() error");

    ShowSockBufSize(sock);
    
    return 0;
}

void ShowSockBufSize(int sock){
    int rcv_buf, snd_buf, len;
    int state;

    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)& rcv_buf, &len);
    if(state) ErrorHandling("getsockopt() error");
    printf("Input buffer size : %d\n", rcv_buf);

    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)& snd_buf, &len);
    if(state) ErrorHandling("getsockopt() error");
    printf("Output buffer size : %d\n", snd_buf);
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}