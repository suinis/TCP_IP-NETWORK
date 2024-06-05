#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]){
    unsigned short host_post = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;

    net_port = htons(host_post);
    net_addr = htonl(host_addr);

    printf("Host port : %#x \n", host_post);
    printf("Network port : %#x \n", net_port);
    printf("Host address : %#lx \n", host_addr);
    printf("Network address : %#lx \n", net_addr);
    return 0;
}