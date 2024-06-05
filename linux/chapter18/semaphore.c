#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

void* read(void* arg);
void* accu(void* arg);
static sem_t sem_one;
static sem_t sem_two;
// static pthread_mutex_t mutex;
static int num; //默认值为0

int main(){
    int cnt = 5;
    pthread_t tr1, tr2;
    sem_init(&sem_one, 0, 0);
    sem_init(&sem_two, 0, 1);
    // pthread_mutex_init(&mutex, NULL);

    pthread_create(&tr1, NULL, read, (void*)&cnt);
    pthread_create(&tr2, NULL, accu, (void*)&cnt);

    pthread_join(tr1, NULL);
    pthread_join(tr2, NULL);

    sem_destroy(&sem_one);
    sem_destroy(&sem_two); 
    // pthread_mutex_destroy(&mutex);

    return 0;
}

void* read(void* arg){
    int cnt = *((int*)arg);
    for(int i = 0; i < cnt; ++i){
        fputs("Input num:", stdout);
        sem_wait(&sem_two);
        // pthread_mutex_lock(&mutex);
        scanf("%d", &num);
        sem_post(&sem_one);
        // pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* accu(void* arg){
    int sum = 0;
    int cnt = *((int*)arg);
    for(int i = 0; i < cnt; ++i){
        sem_wait(&sem_one);
        // pthread_mutex_lock(&mutex);
        sum += num;
        sem_post(&sem_two);
        // pthread_mutex_unlock(&mutex);
    }
    printf("sum = %d \n", sum);
    return NULL;
}