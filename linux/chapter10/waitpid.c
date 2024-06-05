#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //fork
#include <sys/wait.h> //waitpid

int main(int argc, char* argv[]){
    int status;
    pid_t pid = fork();

    if(pid == 0){
        sleep(15);
        return 7;
    }else{
        while(!waitpid(-1, &status, WNOHANG)){ //设置第一位为-1，如果有任意的子进程终止，waitpid会返回对应终止的子进程ID，也就退出循环

            sleep(3);
            puts("sleep 3sec.");
        }

        if(WIFEXITED(status))
            printf("Child send %d \n", WEXITSTATUS(status));
    }
    return 0;
}