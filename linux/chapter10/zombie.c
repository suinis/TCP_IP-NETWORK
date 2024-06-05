#include <stdio.h>
#include <unistd.h>
#include <wait.h> //pid_t

int main(int argc, char* argv[]){
    pid_t pid;

    pid = fork();

    if(pid == 0) printf("Hi, Iam a child Process\n");
    else{
        printf("Child Process ID : %d\n", pid);
        sleep(30); //父进程等待30s
    }

    if(pid == 0) printf("End child process\n");
    else printf("End parent process\n");
    return 0;
}