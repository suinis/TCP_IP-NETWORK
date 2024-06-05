#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#define BUFSIZE 1024

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int clnt_sock;
    struct sockaddr_in serv_addr;
    ssize_t read_len;
    char buf[BUFSIZE];

    if(argc != 3){
        printf("Usage : %s <IP> <port> \n", argv[0]);
    }

    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(clnt_sock == -1){
        ErrorHandling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        ErrorHandling("connect() error");
    }
    
    while (1)
    {
        fputs("Input message(Q to quit): ", stdout);
        fgets(buf, BUFSIZE, stdin);
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")){
            break;
        }
        write(clnt_sock, buf, strlen(buf));
        //因为EPETserv服务器端设置buf空间很小，服务器端传输回声数据到客户端需要多次write，
        //如果客户端在write传输数据后立刻调用read，可能会在服务器端没有传输完全部数据前读取数据，
        //导致只读取部分回声数据
        sleep(1);  

        read_len = read(clnt_sock, buf, BUFSIZE-1);
        buf[read_len] = 0; // (char)0 -> '\0' 
        printf("message from server : %s \n", buf);
    }
    
    close(clnt_sock);
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
