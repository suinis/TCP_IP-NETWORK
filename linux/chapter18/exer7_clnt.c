#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 1024

void error_handling(const char* msg);
void* send_msg(void* arg);
void* recv_msg(void* arg);

int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in serv_adr;
    pthread_t t_send, t_recv;
    void* thread_return;

    if(argc != 3){
        printf("Usage : %s <IP> <PORT> \n", argv[0]);
        exit(0);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)& serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error");

    pthread_create(&t_send, NULL, send_msg, (void*)&sock);
    pthread_create(&t_recv, NULL, recv_msg, (void*)&sock);
    pthread_join(t_send, NULL);
    pthread_join(t_recv, NULL);

    close(sock);

    return 0;
}

void error_handling(const char* msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(0);
}

void* send_msg(void* arg){
    int sock = *((int*)arg);
    char msg[BUFSIZE];
    while (1)
    {
        // fputs("Input message :", stdout);
        fgets(msg, BUFSIZE, stdin);
        if(!strncmp(msg, "q\n", 2) || !strncmp(msg, "Q\n", 2)){
            close(sock);
            exit(0);
        }
        write(sock, msg, strlen(msg));
    }
    return NULL;
}

void* recv_msg(void* arg){
    int sock = *((int*)arg);
    int str_len;
    char msg[BUFSIZE];
    while ((str_len = read(sock, msg, BUFSIZE - 1)) > 0)
    {
        msg[str_len] = 0;
        fputs(msg, stdout);
    }
    return NULL;
}