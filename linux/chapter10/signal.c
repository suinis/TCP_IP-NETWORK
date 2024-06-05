#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void timeout(int sig){
    if(sig == SIGALRM){
        puts("Time out!");
    }
    alarm(2);
}

void keycontrol(int sig){
    if(sig == SIGINT){
        puts("ctrl + c pressed!");
    }
}

int main(int argc, char* argv[]){
    signal(SIGINT, keycontrol);
    signal(SIGALRM, timeout);
    // alarm(2); 

    for(int i = 0; i < 3; ++i){
        puts("wait..."); 
        sleep(100); //如果将main函数中alarm(2)注释掉，在运行窗口键入ctrl + c也可以实现终止进程的sleep状态
        //说明：发送信号时，将唤醒调用sleep进入阻塞态的进程
        sleep(100);
    }
    return 0;
}