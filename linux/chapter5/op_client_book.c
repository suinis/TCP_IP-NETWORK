#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in addr;
    char opmsg[BUFSIZE];
    int result = 0, opnd_cnt, i;

    if(argc != 3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        ErrorHandling("socket() error");
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1){
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
    if(write(sock, opmsg, 1 + opnd_cnt * OPSZ + 1) == -1){
        ErrorHandling("write() error");
    }
    if(read(sock, &result, RLT_SIZE) == -1){
        ErrorHandling("read() error");
    }else{
        printf("Operation result: %d\n", result);
    }
    close(sock);

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}