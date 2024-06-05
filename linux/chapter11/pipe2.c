#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

#define BUFSIZE 50

void ErrorHandling(const char* message);

int main(int argc, char*argv[]){
    int read_len;
    int fds[2];
    char str1[] = "Who are you?";
    char str2[] = "Thank you for your message!";
    char buf[BUFSIZE];

    pid_t pid;
    pipe(fds);

    pid = fork();
    if(pid == 0){
        write(fds[1], str1, sizeof(str1));

        // sleep(2); //

        //接收父进程数据
        read_len = read(fds[0], buf, BUFSIZE);
        if(read_len == -1) ErrorHandling("Child pro read() error");
        buf[read_len] = 0; 
        printf("Child pro output : %s \n", buf);
    }else{
        //接收子进程数据
        read_len = read(fds[0], buf, BUFSIZE);
        if(read_len == -1) ErrorHandling("Parent pro read() error");
        buf[read_len] = 0; 
        printf("Parent pro output : %s \n", buf);

        write(fds[1], str2, sizeof(str2));

        sleep(3); //等待子进程执行完毕，主进程再退出
    }
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}