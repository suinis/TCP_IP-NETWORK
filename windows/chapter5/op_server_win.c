#include <stdio.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void ErrorHandling(const char* message);

int caculateResult(int opnum, int opnds[], char op);

int main(int argc, char* argv[]){
    WSADATA wsaData;
    int serv_sock, clnt_sock;
    SOCKADDR_IN serv_addr, clnt_addr;
    char opmsg[BUFSIZE];
    int opnd_cnt, read_len, result = 0;
    char op;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        ErrorHandling("WSAStartup() error");
    }
    
    if(argc != 2){
        printf("Usage : %s <port> \n", argv[0]);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == INVALID_SOCKET){
        ErrorHandling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR){
        ErrorHandling("bind() error");
    }

    if(listen(serv_sock, 5) == SOCKET_ERROR){
        ErrorHandling("listen() error");
    }

    int clnt_addr_sz = sizeof(clnt_addr);
    for(int i = 0; i < 5; ++i){
        clnt_sock = accept(serv_sock, (SOCKADDR*)&clnt_addr, &clnt_addr_sz);
        if(clnt_sock == INVALID_SOCKET){
            closesocket(serv_sock);
            closesocket(clnt_sock);
            ErrorHandling("accept() error");
        }else{
            printf("\nconnected to client%d...\n", i + 1);
        }

        if((read_len = recv(clnt_sock, opmsg, BUFSIZE - 1, 0)) == SOCKET_ERROR){
            ErrorHandling("read() error");
        }
        printf("read_len = %d\n", read_len);
        //第一个参数为opnd_cnt = (int)opmsg[0]，第一个字节客户端设置为存储接下来要传输的num的个数
        opnd_cnt = (int)opmsg[0];
        //第二个参数(int*)&opmsg[1]，第二个字节开始客户端设置为每四个字节为一个int型数据
        //第三个参数opmsg[1 + opnd_cnt * OPSZ]，客户端设置为一个字节的op操作符
        result = caculateResult(opnd_cnt, (int*)&opmsg[1], opmsg[1 + opnd_cnt * OPSZ]);

        if(send(clnt_sock, (CHAR*)&result, OPSZ, 0) == SOCKET_ERROR){
            ErrorHandling("write() error");
        }

        closesocket(clnt_sock);
    }
    
    closesocket(serv_sock);
    WSACleanup();
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