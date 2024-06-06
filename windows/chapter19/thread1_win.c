#include <stdio.h>
#include <Windows.h>
#include <process.h>

unsigned WINAPI ThreadFunc(void* arg);

int main(int agrc, char* argv[]){
    HANDLE hThread;
    unsigned threadID;
    int param = 5;

    hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, &param, 0, &threadID);
    if(hThread == NULL){
        puts("_beginThreadex() error");
        return -1;
    }
    Sleep(3000);
    puts("end of main()");
    return 0;
}

unsigned WINAPI ThreadFunc(void* arg){
    int i;
    int cnt = *((int*)arg);

    for(i=0; i<cnt; i++){
        Sleep(1000);
        printf("ThreadFunc() running %d\n", i);
    }
    return 0;
}