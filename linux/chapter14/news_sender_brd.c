#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFSIZE 30

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int send_sock;
    char buf[BUFSIZE];
    size_t fread_len;
    struct sockaddr_in broadcase_adr;
    FILE* fp;
    int bcast = 1;

    if(argc != 3){
        printf("Usage : %s <IP> <PORT> \n", argv[0]);
        exit(1);
    }

    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(send_sock == -1)
        ErrorHandling("socket() error");
    
    memset(&broadcase_adr, 0, sizeof(broadcase_adr));
    broadcase_adr.sin_family = AF_INET;
    broadcase_adr.sin_addr.s_addr = inet_addr(argv[1]);
    broadcase_adr.sin_port = htons(atoi(argv[2]));

    if(setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*)&bcast, sizeof(bcast)) == -1)
        ErrorHandling("setsockopt() error");

    if((fp = fopen("news.txt", "r")) == NULL)
        ErrorHandling("fopen() error");

    while(!feof(fp)){
        //fgets 更适合用于读取文本文件并处理行数据，而 fread 更适合用于读取二进制文件或按字节读取数据。
        fgets(buf, BUFSIZE, fp); //会带上换行符'\n'，如果不需要的话可以传输时strlen(buf) - 1，或者通过sprintf直接格式化改变buf内容
        //sprintf() 函数在写入字符数组时不会检查目标缓冲区的大小，因此需要确保目标缓冲区足够大以容纳格式化后的字符串，以防止发生缓冲区溢出。

        // fread(buf, 1, BUFSIZE, fp);
        // fread_len = strlen(buf);
        // buf[fread_len] = 0;
        sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)& broadcase_adr, sizeof(broadcase_adr));
        printf("message : %s \n", buf);
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