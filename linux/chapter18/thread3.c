#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

void* thread_main(void* arg);

int main(){
    pthread_t t_id1, t_id2;
    int* sum1_p, *sum2_p;
    int range1[] = {1, 5};
    int range2[] = {6, 10};

    pthread_create(&t_id1, NULL, thread_main, (void*) range1);
    pthread_create(&t_id2, NULL, thread_main, (void*) range2);

    pthread_join(t_id1, (void**)&sum1_p);
    pthread_join(t_id2, (void**)&sum2_p);
    printf("add 1 to 10 : %d \n", *sum1_p + *sum2_p);
    // printf("add 1 to 10 : %d \n", sum1_p[0] + sum2_p[0]);
    free(sum1_p);
    free(sum2_p);

    return 0;
}

void* thread_main(void* arg){
    int start = ((int*)arg)[0];
    int end = ((int*)arg)[1];
    int* sum_p = malloc(sizeof(int));
    *sum_p = 0;
    for(int i = start; i <= end; ++i){
        *sum_p += i;
    }
    return (void*)sum_p;
}