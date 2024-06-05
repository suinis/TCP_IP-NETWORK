#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 100

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    char message[BUFSIZE];
    
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

    int num_cnt, num_cnt_cp;
    int get_num_arr[BUFSIZE];
    int result = 0, idx = 0;
    char op;
    if(read(clnt_sock, &num_cnt, 4) == -1){ //获取客户端发送的num个数
        ErrorHandling("read() error");
    } 
    num_cnt_cp = num_cnt;
    while (num_cnt_cp--) //获取所有num
    {
        if(read(clnt_sock, &get_num_arr[idx++], 4) == -1){
            ErrorHandling("read() error");
        }
    }
    if(read(clnt_sock, &op, 1) == -1){ //获取操作符
        ErrorHandling("read() error");
    }
    switch (op)
    {
    case '+':
        for(int i = 0; i < num_cnt; ++i){
            result += get_num_arr[i];
        }
        break;
    case '-':
        result = get_num_arr[0];
        for(int i = 1; i < num_cnt; ++i){
            result -= get_num_arr[i];
        }
        break;
    case '*':
    result = get_num_arr[0];
        for(int i = 1; i < num_cnt; ++i){
            result *= get_num_arr[i];
        }
        break;
    default:
        break;
    }
    printf("result : %d\n", result);

    if(write(clnt_sock, &result, 4) == -1){
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