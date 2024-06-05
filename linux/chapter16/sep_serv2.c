#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BUFSIZE 1024

int main(int argc, char* argv[]){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    FILE* writefp;
    FILE* readfp;
    int clnt_adr_sz;
    char buf[BUFSIZE] = {0,};

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family =  AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    bind(serv_sock, (struct sockaddr*)& serv_adr, sizeof(serv_adr));
    listen(serv_sock, 5);

    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)& clnt_adr, &clnt_adr_sz);

    writefp = fdopen(clnt_sock, "w");
    readfp = fdopen(dup(clnt_sock), "r"); //调用dup复制一个文件描述符，同时指向该套接字

    fputs("From server : hi client? \n", writefp);
    fputs("I love all of the world \n", writefp);
    fputs("You are awesome! \n", writefp);
    fflush(writefp);

    shutdown(fileno(writefp), SHUT_WR); //半关闭输出流
    fclose(writefp);
    fgets(buf, BUFSIZE, readfp); //验证关闭writefp后是否能继续使用readfp接收数据
    fputs(buf, stdout);
    fclose(readfp);

    return 0;
}