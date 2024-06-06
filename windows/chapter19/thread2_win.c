#include <stdio.h>
#include <Windows.h>
#include <process.h>

unsigned WINAPI ThreadFunc(void* arg);

int main(int argc, char* argv[]){
    HANDLE hThread;
    DWORD wr;
    unsigned threadId;
    int param = 5;

    hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (void*)&param, 0, &threadId);
    
    if(hThread == 0){
        puts("_beginthreadex() error");
        return -1;
    }
    
    if((wr = WaitForSingleObject(hThread, INFINITE)) == WAIT_FAILED){
        puts("WaitForSingleObject() error");
        return -1;
    }

    printf("wait result : %s\n", (wr == WAIT_OBJECT_0) ? "signaled" : "timeout");
    puts("end of main");
    return 0;

}

unsigned WINAPI ThreadFunc(void* arg){
    int i;
    int cnt = (*(int*)arg);
    for(i = 0; i < cnt; ++i){
        Sleep(1000);
        printf("thread counting%d\n", i);   
    }
    return 0;
}
