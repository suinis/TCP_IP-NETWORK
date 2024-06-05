#include <stdio.h>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>

#define BUFSIZE 50

int main(int argc, char* argv[]){
    fd_set reads, temps;
    struct timeval timeout;
    int result, str_len;
    char buf[BUFSIZE];

    FD_ZERO(&reads);
    FD_SET(0, &reads);

    // timeout.tv_sec = 5;
    // timeout.tv_usec = 0;

    while (1)
    {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        //每次执行完select函数后，timeout都会被赋值为超时前剩余时间，
        //也即如果不在while循环内部设置timeout值，则当超时时间耗尽，后续timeout值均被设置为0s.会一直执行Time-out!输出
        result = select(1, &temps, 0, 0, &timeout);  //设置监听的描述符范围：0 ~ 0
        
        if(result == -1){
            fputs("select() error", stderr);
            break;
        }
        else if(result == 0){
            puts("Time-out!");
        }
        else{
            if(FD_ISSET(0, &temps))
            {
                str_len = read(0, buf, BUFSIZE);
                buf[str_len] = 0;
                printf("message from console : %s\n", buf);
            }
        }
    }

    return 0;
}