#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in addr;
    int num_cnt, result;
    char op;

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
    scanf("%d", &num_cnt);
    if(write(sock, &num_cnt, 4) == -1){
        ErrorHandling("write() error");
    }
    for(int i = 0; i < num_cnt; ++i){
        int cnt_num;
        printf("Operand %d:", i + 1);
        scanf("%d", &cnt_num);
        if(write(sock, &cnt_num, 4) == -1){
            ErrorHandling("write() error");
        }
    }
    fgetc(stdin); //必须要写，清除掉输入缓冲区中的一个字符，可能时scanf输入后的回车符
    fputs("Operator:", stdout);
    scanf("%c", &op);
    if(write(sock, &op, 1) == -1){
        ErrorHandling("write() error");
    }
    if(read(sock, &result, 4) == -1){
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