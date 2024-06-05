#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUFSIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    WSADATA wsaData;
    int sock;
    SOCKADDR_IN addr;
    char opmsg[BUFSIZE];
    int result = 0, opnd_cnt, i;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        ErrorHandling("WSAStartup() error");
    }

    if(argc != 3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET){
        ErrorHandling("socket() error");
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR){
        ErrorHandling("connect() error");
    }else{
        printf("connected...\n");
    }

    fputs("Operand count: ", stdout);
    scanf("%d", &opnd_cnt);
    opmsg[0] = (char)opnd_cnt;
    for(int i = 0; i < opnd_cnt; ++i){
        printf("Operand %d:", i + 1);
        scanf("%d", (int*)&opmsg[1 + i * OPSZ]);
    }
    fgetc(stdin); //必须要写，清除掉输入缓冲区中的一个字符，可能时scanf输入后的回车符
    fputs("Operator:", stdout);
    scanf("%c", &opmsg[1 + opnd_cnt * OPSZ]);
    if(send(sock, opmsg, 1 + opnd_cnt * OPSZ + 1, 0) == SOCKET_ERROR){
        ErrorHandling("write() error");
    }
    if(recv(sock, (CHAR*)&result, RLT_SIZE, 0) == -SOCKET_ERROR){
        ErrorHandling("read() error");
    }else{
        printf("Operation result: %d\n", result);
    }
    closesocket(sock);
    WSACleanup();

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}