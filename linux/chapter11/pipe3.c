#include <stdio.h>
#include <unistd.h>
#include <wait.h>

#define BUFSIZE 50

int main(int argc, char* argv[]){
    int fds1[2], fds2[2];
    pid_t pid;
    char str1[] = "who are you?";
    char str2[] = "Thank you for your message!";
    char buf[BUFSIZE];

    pipe(fds1); pipe(fds2);
    pid = fork();

    if(pid == 0) {
        write(fds1[1], str1, sizeof(str1));

        read(fds2[0], buf, BUFSIZE);
        printf("Child pro output : %s\n", buf); //printf会自动识别字符串末尾，自动添加结束符'\0'
    }else{
        read(fds1[0], buf, BUFSIZE);
        printf("Parent pro output : %s\n", buf);

        write(fds2[1], str2, sizeof(str2));

        sleep(3); //防止父进程先结束
    }
    return 0;
}