#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <signal.h>

//2.通过signal信号机制管控子进程终止事件，观察主进程何时执行自己代码
void terminalChild(int sig){
    int status;
    pid_t closed_pid;
    if (sig == SIGCHLD)
    {
        //调用了子进程终止处理函数，意味着必定有一个子进程终止
        closed_pid = waitpid(-1, &status, WNOHANG);
        if(WIFEXITED(status))
            printf("pid : %d closed, return : %d!\n", closed_pid, WEXITSTATUS(status));
    }
}

int main(int argc, char* argv[]){
    pid_t pid = fork();
    pid_t closed_pid;
    int status;

    if(pid == 0){ //子进程执行
        printf("child thread sleep 15secs...\n");
        sleep(15);
        return 15;
    }else{
        //1.通过while循环持续调用waitpid，观察何时主进程可以执行自己代码。
        // while (!(closed_pid = waitpid(pid, &status, WNOHANG)))
        // {
        //     sleep(3);
        //     printf("sleep 3sec...\n");
        // }
        // printf("pid : %d closed!\n", closed_pid);

        //2.通过signal绑定信号处理函数
        // signal(SIGCHLD, terminalChild);

        //3.通过sigaction进行信号处理
        struct sigaction act;
        act.sa_handler = terminalChild;
        // 在sigaction结构中，sa_mask字段是一个信号集，它指定了在信号处理函数执行期间应该被阻塞的信号。
        // 如果在调用信号处理函数时，有信号在sa_mask中被设置，那么这些信号在信号处理函数执行期间将被阻塞，不会被递送给该进程。
        sigemptyset(&act.sa_mask); //将信号集sa_mask置空。
        act.sa_flags = 0;
        sigaction(SIGCHLD, &act, 0);

        //模拟主进程后续执行需要20s
        printf("Able to get into next part!\n");
        sleep(20); //主进程休眠20s，
        
    }

    return 0;
}