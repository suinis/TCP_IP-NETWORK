#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

void ErrorHandling(const char* message);

void printfAddr(const char* addr);

int main(int argc, char* argv[]){
    char* str_ptr;
    char str_arr[20];
    struct sockaddr_in sockaddr1, sockaddr2;
    sockaddr1.sin_addr.s_addr = htonl(0x1020304);
    sockaddr2.sin_addr.s_addr = htonl(0x1010101);

    str_ptr = inet_ntoa(sockaddr1.sin_addr); //目前指向addr1
    strcpy(str_arr, str_ptr); //记录addr1
    printfAddr(str_ptr);
    inet_ntoa(sockaddr2.sin_addr); //就算不赋值，str_ptr指向也会改变，目前指向addr2
    printfAddr(str_ptr);
    printfAddr(str_arr);

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void printfAddr(const char* addr){
    if(addr == "-1"){
        ErrorHandling("inet_ntoa() error");
    }else{
        printf("address:%s\n", addr);
    }
}