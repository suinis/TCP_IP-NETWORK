#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 100
#define NAME_SIZE 20

void error_handling(const char* msg);
void* send_msg(void* arg);
void* recv_msg(void* arg);
char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUFSIZE];

int main(int argc, char* argv[]){
    int clnt_sock;
    struct sockaddr_in serv_adr;
    pthread_t t_send, t_recv;
    void* thread_return;

    if(argc != 4){
        printf("Usage : %s <IP> <PORT> <NAME> \n", argv[0]);
        exit(0);
    }

    sprintf(name, "[%s]", argv[3]);
    if((clnt_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        error_handling("socket() error");
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(clnt_sock, (struct sockaddr*)& serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error");
    
    pthread_create(&t_send, NULL, send_msg, (void*)&clnt_sock);
    pthread_create(&t_recv, NULL, recv_msg, (void*)&clnt_sock);
    pthread_join(t_send, (void**)&thread_return);
    pthread_join(t_recv, (void**)&thread_return);
    close(clnt_sock);

    return 0;
}

void* send_msg(void* arg){
    int sock = (*(int*)arg);
    char name_msg[NAME_SIZE + BUFSIZE];
    while (1)
    {
        fgets(msg, BUFSIZE, stdin);
        if(!strncmp(msg, "q\n", 2) || !strncmp(msg, "Q\n", 2)){
            close(sock);
            exit(0);
        }
        sprintf(name_msg, "%s %s", name, msg); //包含msg的回车符
        write(sock, name_msg, strlen(name_msg));
    }
    return NULL;
}

void* recv_msg(void* arg){
    int sock = (*(int*)arg);
    int str_len;
    while (1)
    {
        char name_msg[NAME_SIZE + BUFSIZE];
        str_len = read(sock, name_msg, NAME_SIZE + BUFSIZE - 1);
        if(str_len == -1)
            return (void*)-1;
        name_msg[str_len] = 0;
        fputs(name_msg, stdout);
    }
    return NULL;
}

void error_handling(const char* msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(0);
}