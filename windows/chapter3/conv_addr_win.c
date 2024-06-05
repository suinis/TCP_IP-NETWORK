// #undef UNICODE
// #undef _UNICODE
#include <stdio.h>
#include <WinSock2.h>

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    WSADATA WSAData;
    if(WSAStartup(MAKEWORD(2, 2), &WSAData) != 0){
        ErrorHandling("WSAStartup() error");
    }

    char* strAddr = "192.168.92.131:8080";
    char strAddrBuf[50];
    // struct sockaddr_in servAddr;
    //typedef struct sockaddr_in SCOKADDR_IN 同一数据类型，只不过为了保持windows socket api命名规范重命名了而已
    SOCKADDR_IN servAddr;
    int size = sizeof(servAddr);

    if(WSAStringToAddress(strAddr, AF_INET, NULL, (SOCKADDR*)&servAddr, &size) == SOCKET_ERROR){
        ErrorHandling("WSAStringToAddress() error");
    }else{
        printf("sockaddr size : %d\n", size);
    }

    size = sizeof(strAddrBuf);
    if(WSAAddressToString((SOCKADDR*)&servAddr, sizeof(servAddr), NULL, strAddrBuf, (DWORD*)&size) == SOCKET_ERROR){ //typedef unsigned long DWORD
        ErrorHandling("WSAAddressToString() error");
    }else{
        printf("second conv result : %s \n", strAddrBuf);
    }

    WSACleanup();
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}