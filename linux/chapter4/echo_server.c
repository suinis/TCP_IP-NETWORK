#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#define BUFSIZE 1024

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int serv_sock, io_sock;
    struct sockaddr_in servaddr;
    struct sockaddr_in clntaddr;
    socklen_t clnt_addr_sz;
    char buf[BUFSIZE];
    ssize_t read_len; 
    int i;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    if((serv_sock = socket(PF_INET, SOCK_STREAM, 0))  == -1){
        ErrorHandling("socket() error");
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&servaddr, sizeof(servaddr))  == -1){
        ErrorHandling("bind() error");
    }
    
    if(listen(serv_sock, 5) == -1){
        ErrorHandling("listen() error");
    }

    clnt_addr_sz = sizeof(clntaddr);
    for(i = 0; i < 5; ++i){
        if((io_sock = accept(serv_sock, (struct sockaddr*)&clntaddr, &clnt_addr_sz)) == -1){
            ErrorHandling("accept() error");
        }else{
            printf("connected client %d \n", i + 1);
        }
        while ((read_len = read(io_sock, buf, BUFSIZE)) != 0){
            write(io_sock, buf, read_len);
        }
        close(io_sock);
    }
    close(serv_sock);

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}