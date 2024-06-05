#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

int sum = 0;

void* thread_main(void* arg);

int main(){
    pthread_t t_id1, t_id2;
    int range1[] = {1, 5};
    int range2[] = {6, 10};

    pthread_create(&t_id1, NULL, thread_main, (void*) range1);
    pthread_create(&t_id2, NULL, thread_main, (void*) range2);

    pthread_join(t_id1, NULL);
    pthread_join(t_id2, NULL);
    printf("add 1 to 10 : %d \n", sum);

    return 0;
}

void* thread_main(void* arg){
    // int start = ((int*)arg)[0];
    // int end = ((int*)arg)[1];
    int* range = malloc(sizeof(arg));
    range = (int*)arg;
    for(int i = range[0]; i <= range[1]; ++i){
        sum += i;
    }
    free(range);
    return NULL;
}