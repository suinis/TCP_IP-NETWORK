#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
// #include <arpa/inet.h>
// #include <unistd.h>

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int tcp_sock, udp_sock;
    int getres;
    int sock_type;
    socklen_t optlen;

    tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    printf("SOCK_STREAM : %d\n", SOCK_STREAM);
    udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
    printf("SOCK_DGRAM : %d\n", SOCK_DGRAM);

    getres = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if(getres == -1) ErrorHandling("getsockopt() error");
    printf("sock_type 1 : %d \n", sock_type);

    getres = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if(getres == -1) ErrorHandling("getsockopt() error");
    printf("sock_type 2 : %d \n", sock_type);

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}