#include <stdio.h>
#include <wait.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    pid_t pid;
    pid = fork();

    //ps -o user,pid,ppid,cmd -ax | grep exer2
    //上述命令查看进程用户，进程id，父进程id，命令行。以此查看子进程在父进程结束后成为孤儿进程后，它的父进程是不是1（init进程）。
    //经验证如下：
    //父进程终止前：
    // info-se+    5591    3980 ./exer2
    // info-se+    5592    5591 ./exer2
    // info-se+    5618    2371 grep --color=auto exer2

    //父进程终止后：
    // info-se+    5592       1 ./exer2
    // info-se+    5671    2371 grep --color=auto exer2

    //发现父进程终止后，子进程成为孤儿进程，其父进程改为init进程(进程ID为1)
    if(pid == 0){
        sleep(40);
        puts("Child pro sleep 20s \n");
    }else{
        sleep(20);
        puts("Parent pro sleep 5s \n");
    }

    return 0;
}