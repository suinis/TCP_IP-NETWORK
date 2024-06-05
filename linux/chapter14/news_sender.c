#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define TTL 64
#define BUFSIZE 30

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int send_sock;
    struct sockaddr_in mul_adr;
    FILE* fp;
    char buf[BUFSIZE];
    int time_live = TTL, get_len;

    if(argc != 3){
        printf("Usage : %s <GROUPIP> <PORT> \n", argv[0]);
        exit(1);
    }

    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&mul_adr, 0, sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]);
    mul_adr.sin_port = htons(atoi(argv[2]));

    if(setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)& time_live, sizeof(time_live)) == -1)
        ErrorHandling("setsockopt() error");

    if((fp = fopen("news.txt", "r")) == NULL)
        ErrorHandling("fopen() error");

    while (!feof(fp))
    {
        // fread(buf, BUFSIZE - 1, 1, fp);
        fgets(buf, BUFSIZE - 1, fp);
        // get_len = strlen(buf);
        // buf[get_len] = 0;
        // printf("message : %s \n", buf);
        sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)& mul_adr, sizeof(mul_adr));
        sleep(2);
    }
    
    fclose(fp);
    close(send_sock);

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}