#include <stdio.h>
#include <WinSock2.h>
#include <sys/types.h>

void ErrorHandling(const char* message);

int main(int argc, char* aargv[]){
    WSADATA wsaData;
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;
 
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){ //task.json中构建设置了编译时链接ws2_32.lib库，这里不写不会报错，但最好写上
        ErrorHandling("WSAStartup() error");
    }

    net_port = htons(host_port);
    net_addr = htonl(host_addr);

    printf("host port:%#x\n", host_port);
    printf("network port:%#x\n", net_port);
    printf("host addr:%#x\n", host_addr);
    printf("network addr:%#x\n", net_addr);

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}