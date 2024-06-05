#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/socket.h>

int main(int argc, char* argv[]){
    int sock;
    pid_t pid;
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    pid = fork();
    if(pid == 0) printf("Child pro sock : %d \n", sock);
    else printf("Parent pro sock : %d \n", sock);

    close(sock);
    return 0;
}