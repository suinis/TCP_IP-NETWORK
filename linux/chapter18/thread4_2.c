#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREAD 100
#define FOR_LOOPNUM 50000000

void* thread_inc(void* arg);
void* thread_dec(void* arg);
//全局变量位于数据区，同一个进程的线程共享数据区和堆区
long long num = 0;
pthread_mutex_t mutex;

int main(){
    pthread_t thread_id[NUM_THREAD];
    int i;

    //初始化互斥量
    pthread_mutex_init(&mutex, NULL);

    printf("sizeof long long : %ld \n", sizeof(long long));
    for(i = 0; i < NUM_THREAD; ++i){
        if(i % 2 == 0)
            pthread_create(&thread_id[i], NULL, thread_inc, NULL);
        else
            pthread_create(&thread_id[i], NULL, thread_dec, NULL);
    }

    for (i = 0; i < NUM_THREAD; ++i)
    {
        pthread_join(thread_id[i], NULL);
    }
    printf("result : %lld \n", num);

    //销毁互斥量
    pthread_mutex_destroy(&mutex);

    return 0;
}

void* thread_inc(void* arg){
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < FOR_LOOPNUM; ++i)
        ++num;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* thread_dec(void* arg){
    // pthread_mutex_lock(&mutex);
    for(int i = 0; i < FOR_LOOPNUM; ++i){
        pthread_mutex_lock(&mutex);
        --num;
        pthread_mutex_unlock(&mutex);
    } 
    // pthread_mutex_unlock(&mutex);
    return NULL;
}