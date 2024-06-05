#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h> //errno
#include <string.h> //strerror

void read_childpro(int sig){
    pid_t pid;
    int status;
    if(sig == SIGCHLD){
        //不加上while循环这一句，在执行时，两个子进程由于sleep时间相同，可能出现同时结束发送SIGCHLD信号。其中一个信号的处理可能覆盖或错过了另一个信号的处理
        while ((pid = waitpid(-1, &status, WNOHANG)) != 0) 
        {
            if (pid == -1) { 
            // 检查errno以确定错误类型
                if (errno == ECHILD) { 
                    //情况1：已经没有子进程了，还执行waitpid会返回-1并设置errno会被设置为ECHILD 
                    //情况2：指定的PID不是调用进程的子进程或者是一个无效的PID
                    puts("No more child processes.");
                } else if (errno == EINTR) {  
                    //waitpid 在等待过程中被信号中断
                    puts("waitpid was interrupted by a signal."); 
                } else { 
                    printf("waitpid failed: %s\n", strerror(errno));
                }
                break;
            }
            puts("read_childpro running...");
            if(WIFEXITED(status)){
                printf("Pemoved pro id: %d\n", pid);
                printf("Child send: %d\n", WEXITSTATUS(status));
            }
        }
    }
}

int main(int argc, char* argv[]){
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childpro;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if(pid == 0){
        puts("Hi! I'm child process");
        sleep(10);
        return(12);
    }else{
        printf("Child pro id: %d\n", pid);

        pid = fork();
        if(pid == 0){
            puts("Hi! I'm child process");
            sleep(10);
            exit(24);
        }else{
            printf("Child pro id: %d\n", pid);

            for(int i = 0; i < 5; ++i){
                puts("wait...");
                sleep(5);
            }
        }
    }
    return 0;
}