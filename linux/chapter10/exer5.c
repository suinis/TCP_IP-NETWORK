#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

int isFinal = 0;

void register_signal_handler();
void handler(int sig);

int main(int argc, char* argv[]){
    int timecnt = 0;
    register_signal_handler();

    while (!isFinal)
    {
        printf("time count : %d \n", timecnt++);
        sleep(1);
    }
    
    return 0;
}

void register_signal_handler(){
    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);
}

void handler(int sig){
    if(sig == SIGINT){
        char choice[3];//或者开为2，但需要将choice[1]位置为'\0'
        puts("terminate pro ? (Y/N) : ");
        fgets(choice, sizeof(choice), stdin);
        // choice[1] = 0;
        if(!strcmp("Y\n", choice) || !strcmp("y\n", choice)) isFinal = 1;
        else puts("Continuing pro...\n");
    }
}