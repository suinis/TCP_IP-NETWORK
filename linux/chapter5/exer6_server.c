#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define FILENAMESIZE 1024
#define FILEBUFSIZE 1024

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    int clnt_addr_sz;
    
    char fileName[FILENAMESIZE];
    char fileBuf[FILEBUFSIZE];
    int fileNameSize;
    FILE* filestream;
    int read_len;
    int write_len;
    int fileSize = 0;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        ErrorHandling("socket() error");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        ErrorHandling("bind() error");
    }

    if(listen(serv_sock, 5) == -1){
        ErrorHandling("listen() error");
    }

    clnt_addr_sz = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
    if(clnt_sock == -1){
        ErrorHandling("accept() error");
    }else{
        printf("connected to client...");
    }

    //获取fileName
    if((fileNameSize = read(clnt_sock, fileName, FILENAMESIZE)) == -1){ //read到输入缓冲，后面不需要再read了，所以就不在客户端先传fileNameSize再传fileName了
        ErrorHandling("read() error");
    }
    fileName[fileNameSize] = 0;
    printf("\nfileName : %s\n", fileName);
    //查找fileName文件
    if((filestream = fopen(fileName, "r")) == NULL){
        close(clnt_sock);
    }else{ //传输fileName文件
        while (1)
        {
            read_len = fread(fileBuf, 1, FILEBUFSIZE, filestream);
            if(read_len == EOF) ErrorHandling("fread() error");
            if(read_len == 0) {
                printf("transport file : '%s' complete! (%d bytes)\n", fileName, fileSize);
                break;
            }
            if((write_len = write(clnt_sock, fileBuf, read_len)) == -1) ErrorHandling("write() error");
            fileSize += write_len;
        }
    }
    fclose(filestream);
    close(serv_sock);
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}