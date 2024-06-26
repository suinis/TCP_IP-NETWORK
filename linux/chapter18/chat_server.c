#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

void* handle_clnt(void* arg);
void send_msg(char* msg, int len);
void error_handling(const char* msg);

int clnt_num = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutex;

int main(int argc, char* argv[]){
    int serv_sock, clnt_sock;
    struct sockaddr_in  serv_adr, clnt_adr;
    int clnt_adr_sz;
    pthread_t t_id;

    if(argc != 2){
        printf("Usage : %s <port> \n", argv[0]);
        exit(0);
    }

    pthread_mutex_init(&mutex, NULL);
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)& serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    clnt_adr_sz = sizeof(clnt_adr);
    while (1)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)& clnt_adr, &clnt_adr_sz);
        pthread_mutex_lock(&mutex);
        clnt_socks[clnt_num++] = clnt_sock;
        pthread_mutex_unlock(&mutex);

        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
        pthread_detach(t_id);
        printf("Connected client IP : %s \n", inet_ntoa(clnt_adr.sin_addr));
    }
    
    close(serv_sock);
    return 0;
}

void* handle_clnt(void* arg){
    int clnt_sock = *((int*)arg);
    int str_len = 0, i;
    char msg[BUF_SIZE];
    while (str_len = read(clnt_sock, msg, BUF_SIZE))
        send_msg(msg, str_len);
    
    pthread_mutex_lock(&mutex);
    for(i = 0; i < clnt_num; ++i){
        if(clnt_sock == clnt_socks[i]){
            // while(i++ < clnt_num - 1){
            //     clnt_socks[i] = clnt_socks[i + 1];
            // }
            while (i < clnt_num){
                clnt_socks[i] = clnt_socks[i + 1];
                ++i;
            }
            break;
        }
    }
    --clnt_num;
    pthread_mutex_unlock(&mutex);
    close(clnt_sock);
}

void send_msg(char* msg, int len){
    int i;
    pthread_mutex_lock(&mutex);
    for(i = 0; i < clnt_num; ++i){
        write(clnt_socks[i], msg, len);
    }
    pthread_mutex_unlock(&mutex);
}

void error_handling(const char* msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(0);
}