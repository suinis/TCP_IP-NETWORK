#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int sock;
    int rcv_buf_sz, snd_buf_sz, state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    len = sizeof(rcv_buf_sz);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)& rcv_buf_sz, &len);
    if(state) ErrorHandling("getsockopt() error");
    printf("Input buffer size : %d\n", rcv_buf_sz);

    len = sizeof(snd_buf_sz);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)& snd_buf_sz, &len);
    if(state) ErrorHandling("getsockopt() error");
    printf("Output buffer size : %d\n", snd_buf_sz);

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}