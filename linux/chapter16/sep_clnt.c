#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BUFSIZE 1024

int main(int argc, char* argv[]){
    int clnt_sock;
    struct sockaddr_in serv_adr;
    FILE* writefp;
    FILE* readfp;
    char buf[BUFSIZE];

    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    connect(clnt_sock, (struct sockaddr*)& serv_adr, sizeof(serv_adr));

    writefp = fdopen(clnt_sock, "w");
    readfp = fdopen(clnt_sock, "r");

    while (1)
    {
        if(fgets(buf, BUFSIZE, readfp) == NULL)
            break;
        fputs(buf, stdout);
        fflush(stdout);
    }
    
    fputs("From client : Thank you! \n", writefp);
    fflush(writefp);
    fclose(writefp);
    fclose(readfp);

    return 0;
}