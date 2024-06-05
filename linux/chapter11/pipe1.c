#include <stdio.h>
#include <unistd.h>
#include <wait.h> //pid_t
#include <stdlib.h> //exit

#define BUFSIZE 30

int main(int argc, char* argv[]){
    int fds[2];
    int read_len;
    char str[] = "Who are you?";
    char buf[BUFSIZE];
    pid_t pid;

    pipe(fds);
    pid = fork();
    
    if(pid == 0){ //子进程传输数据给父进程
        write(fds[1], str, sizeof(str));
    }else{  
        read_len = read(fds[0], buf, BUFSIZE);
        if(read_len == -1){
            fputs("read() error", stderr);
            fputc('\n', stderr);
            exit(1);
        }
        buf[read_len] = 0;
        puts(buf);
    }
    return 0;
}