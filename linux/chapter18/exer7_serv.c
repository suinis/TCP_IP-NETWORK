#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 1024
#define CLNT_NUM 256

void error_handling(const char* msg);
void* handle_msg(void* arg);

pthread_mutex_t mutex;
char msgs[BUFSIZE];
int clnt_socks[CLNT_NUM];
int clnt_num = 0;

int main(int argc, char* argv[]){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int clnt_Adr_sz;
    pthread_t t_id;

    if(argc != 2){
        printf("Usage : %s <PORT> \n", argv[0]);
        exit(0);
    }

    pthread_mutex_init(&mutex, NULL); //初始化互斥量
    if((serv_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)& serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    clnt_Adr_sz = sizeof(clnt_adr);
    while (1)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)& clnt_adr, &clnt_Adr_sz);
        printf("connected to client...\n");
        pthread_mutex_lock(&mutex);
        clnt_socks[clnt_num++] = clnt_sock;
        pthread_mutex_unlock(&mutex);

        pthread_create(&t_id, NULL, handle_msg, (void*)&clnt_sock);
        pthread_detach(t_id);
    }
    
    pthread_mutex_destroy(&mutex); //销毁互斥量
    close(serv_sock);

    return 0;
}

void error_handling(const char* msg){
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(0);
}

void* handle_msg(void* arg){
    int clnt_sock = *((int*)arg);
    int str_len, i;
    // char buf[BUFSIZE];
    
    //1.同步
    // pthread_mutex_lock(&mutex);
    // while((str_len = read(clnt_sock, msgs, BUFSIZE - 1)) > 0){
    //     msgs[str_len] = 0;
    //     printf("Received message : %s", msgs);
    //     write(clnt_sock, msgs, str_len);
    // }
    // pthread_mutex_unlock(&mutex);

    //2.不同步
    while((str_len = read(clnt_sock, msgs, BUFSIZE - 1)) > 0){
        msgs[str_len] = 0;
        // sleep(3); // 用来模拟两个客户端同时发送数据，客户端A传输数据后会sleep，
        //在这5秒内，客户端B传输数据，客户端A再读取共享数据msgs回声数据，会发现是客户端B的数据。
        printf("Received message : %s", msgs);
        write(clnt_sock, msgs, str_len);
    }

    //3.修改后的同步，完美实现，但又显得多余
    // while((str_len = read(clnt_sock, buf, BUFSIZE - 1)) > 0){
    //     buf[str_len] = 0;

    //     pthread_mutex_lock(&mutex);
    //     strncpy(msgs, buf, str_len);
    //     msgs[str_len] = 0;
    //     printf("Received message : %s", msgs);
    //     write(clnt_sock, msgs, str_len);
    //     pthread_mutex_unlock(&mutex);
    // }
    // pthread_mutex_lock(&mutex);

    pthread_mutex_lock(&mutex);
    for(i = 0; i < clnt_num; ++i){
        if(clnt_sock == clnt_socks[i]){
            while (i < clnt_num){
                clnt_socks[i] = clnt_socks[i + 1];
                ++i;
            }
            break;
        }
    }
    clnt_num--;
    pthread_mutex_unlock(&mutex);

    printf("closed client...\n");
    close(clnt_sock);
    return NULL;
}