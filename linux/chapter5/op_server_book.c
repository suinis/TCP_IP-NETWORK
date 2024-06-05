#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void ErrorHandling(const char* message);

int caculateResult(int opnum, int opnds[], char op);

int main(int argc, char* argv[]){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    char opmsg[BUFSIZE];
    int opnd_cnt, read_len, result = 0;
    char op;
    
    if(argc != 2){
        printf("Usage : %s <port> \n", argv[0]);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        ErrorHandling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        ErrorHandling("bind() error");
    }

    if(listen(serv_sock, 5) == -1){
        ErrorHandling("listen() error");
    }

    socklen_t clnt_addr_sz = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
    if(clnt_sock == -1){
        close(serv_sock);
        close(clnt_sock);
        ErrorHandling("accept() error");
    }else{
        printf("connected to client...\n");
    }

    if((read_len = read(clnt_sock, opmsg, BUFSIZE - 1)) == -1){
        ErrorHandling("read() error");
    }
    // opmsg[read_len] = 0;
    printf("read_len = %d\n", read_len);
    //第一个参数为opnd_cnt = (int)opmsg[0]，第一个字节客户端设置为存储接下来要传输的num的个数
    opnd_cnt = (int)opmsg[0];
    //第二个参数(int*)&opmsg[1]，第二个字节开始客户端设置为每四个字节为一个int型数据
    //第三个参数opmsg[1 + opnd_cnt * OPSZ]，客户端设置为一个字节的op操作符
    result = caculateResult(opnd_cnt, (int*)&opmsg[1], opmsg[1 + opnd_cnt * OPSZ]);

    if(write(clnt_sock, &result, OPSZ) == -1){
        ErrorHandling("write() error");
    }

    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int caculateResult(int opnum, int opnds[], char op){
    if(opnum < 1) return 0;
    int result = opnds[0];
    switch (op)
    {
    case '+':
        for(int i = 1; i < opnum; ++i){
            result += opnds[i];
        }
        break;
    case '-':
        for(int i = 1; i < opnum; ++i){
            result -= opnds[i];
        }
        break;
    case '*':
        for(int i = 1; i < opnum; ++i){
            result *= opnds[i];
        }
        break;
    default:
        break;
    }
    printf("opnum = %d\n", opnum);
    printf("opnds : ");
    for(int i = 0; i < opnum; ++i){
        printf("%d ", opnds[i]);
    }
    printf("\nop : %c\n", op);
    printf("result : %d\n", result);
    return result;
}