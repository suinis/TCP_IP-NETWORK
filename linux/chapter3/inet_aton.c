#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    char* addr = "1.2.3.4";
    struct sockaddr_in sockaddr;

    if(!inet_aton(addr, &sockaddr.sin_addr)){
        ErrorHandling("inet_aton() error");
    }else{
        printf("Network ordered integer addr: %#x\n", sockaddr.sin_addr.s_addr);
    }

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}