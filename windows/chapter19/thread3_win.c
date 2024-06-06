#include <stdio.h>
#include <Windows.h>
#include <process.h>
#include <stdlib.h>

#define thread_num 50

unsigned WINAPI threadInc(void* arg);
unsigned WINAPI threaddes(void* arg);
long long sum = 0;

int main(int argc, char* argv[]){
    DWORD wr;
    HANDLE hthreads[thread_num];
    unsigned thread_id;
    printf("sizeof long long : %lld\n", sizeof(long long));
    for(int i = 0; i < thread_num; ++i){
        if(i % 2 == 0){
            hthreads[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc, NULL, 0, &thread_id);
            continue;
        }
        hthreads[i] = (HANDLE)_beginthreadex(NULL, 0, threaddes, NULL, 0, &thread_id);
    }

    wr = WaitForMultipleObjects(thread_num, hthreads, TRUE, INFINITE);
    if(wr == WAIT_FAILED){
        puts("WaitFroMultiOnjects() error");
        return -1;
    }

    printf("sum = %lld\n", sum);
    return 0;

}

unsigned WINAPI threadInc(void* arg){
    int i;
    for(i = 0; i < 50000000; ++i){
        sum += 1;
    }
    return 0;
}

unsigned WINAPI threaddes(void* arg){
    int i;
    for(i = 0; i < 50000000; ++i){
        sum -= 1;
    }
    return 0;
}

