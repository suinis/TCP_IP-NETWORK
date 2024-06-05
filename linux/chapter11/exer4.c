#include <stdio.h>
#include <wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 100

void ErrorHandling(const char* message);
void WriteToPipe(const pid_t*, const int*, const char*);
void ReadFromPipe(const pid_t*, const int*, char*);

int main(int argc, char* argv[]){
    int fds[2];
    pid_t pid;
    char str1[] = "Hello Parent pro!";
    char str2[] = "Oh, my child!";
    char str3[] = "Yeah, it's me!";
    char buf[BUFSIZE];

    pipe(fds);
    pid = fork();
    if(pid == 0){ //10s
        WriteToPipe(&pid, &fds[1], str1); //5s
        ReadFromPipe(&pid, &fds[0], buf);
        WriteToPipe(&pid, &fds[1], str3); //5s
        return 0;
    }else{ //10s
        ReadFromPipe(&pid, &fds[0], buf);
        WriteToPipe(&pid, &fds[1], str2); //5s
        ReadFromPipe(&pid, &fds[0], buf);
        sleep(5); //等待子进程结束后，主进程再结束，防止产生孤儿进程，虽说影响不大
    }
    
    return 0;
}


void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void WriteToPipe(const pid_t* pid, const int* sock, const char* str){
    // printf("write : %s\n", str);
    if(write(*sock, str, strlen(str)) == -1) ErrorHandling((*pid == 0) ? "Child pro write() error" : "Parent pro write() error");
    sleep(5); //设置时间不能太短，因为ReadFromPipe函数中没有sleep，有点极限
}

void ReadFromPipe(const pid_t* pid, const int* sock, char* buf){
    int read_len;
    if((read_len = read(*sock, buf, BUFSIZE - 1)) == -1) ErrorHandling((*pid == 0) ? "Child pro read() error" : "Parent pro read() error");
    buf[read_len] = 0;
    printf((*pid == 0) ? "Message from Parent pro : %s\n" : "Message from Child pro : %s\n", buf);
}