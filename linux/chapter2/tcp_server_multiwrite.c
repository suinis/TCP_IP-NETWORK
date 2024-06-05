#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void Error_Handling(const char* message);

int main(int argc, char* argv[]){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int idx = 0;
    ssize_t wirte_len;
    socklen_t clnt_addr_size; 
    char message[] = "hello world";

    if(argc != 2){
        printf("Usage:%s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        Error_Handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)& serv_addr, sizeof(serv_addr)) == -1){
        close(serv_sock);
        close(clnt_sock);
        Error_Handling("bind() error");
    }

    if(listen(serv_sock, 5) == -1){
        close(serv_sock);
        close(clnt_sock);
        Error_Handling("listen() error");
    }

    clnt_addr_size = sizeof(clnt_addr);
    if((clnt_sock = accept(serv_sock, (struct sockaddr*)& clnt_addr, &clnt_addr_size)) == -1){
        close(serv_sock);
        close(clnt_sock);
        Error_Handling("accept() error");
    }

    while ((idx < sizeof(message)) && (wirte_len = write(clnt_sock, &message[idx++], 1)))
    {
        if(wirte_len == -1){
            close(serv_sock);
            close(clnt_sock);
            Error_Handling("write() error");
        }
        printf("have input %d char \n", idx);
    }
    close(serv_sock);
    close(clnt_sock);

    return 0;
}

void Error_Handling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}