#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 30

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int serv_fd, clnt_sd;
    FILE* fp;
    char buf[BUFSIZE];
    int read_cnt;

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    fp = fopen("file_server.c", "rb");
    serv_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_fd == -1){
        ErrorHandling("TCP socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_fd, (struct sockaddr*)& serv_addr, sizeof(serv_addr)) == -1){
        ErrorHandling("bind() error");
    }

    if(listen(serv_fd, 5) == -1){
        ErrorHandling("listen() error");
    }

    clnt_addr_sz = sizeof(clnt_addr);
    clnt_sd = accept(serv_fd, (struct sockaddr*)& clnt_addr, &clnt_addr_sz);
    if(clnt_sd == -1){
        ErrorHandling("accept() error");
    }

    while (1)
    {
        read_cnt = fread((void*)buf, 1, BUFSIZE, fp);
        if(read_cnt == -1) ErrorHandling("fread() error");
        if(read_cnt < BUFSIZE){
            if(write(clnt_sd, buf, read_cnt) == -1) ErrorHandling("write() error");
            break;
        }
        if(write(clnt_sd, buf, BUFSIZE) == -1) ErrorHandling("write() error");
    }
    
    shutdown(clnt_sd, SHUT_WR);
    read_cnt = read(clnt_sd, buf, BUFSIZE - 1);
    if (read_cnt == -1) ErrorHandling("read() error");
    buf[read_cnt] = 0;
    printf("Message from client : %s \n", buf);
    fclose(fp);
    close(clnt_sd);
    close(serv_fd);

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}