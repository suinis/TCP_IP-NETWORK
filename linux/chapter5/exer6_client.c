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
    int sock;
    struct sockaddr_in serv_addr;
    char fileName[FILENAMESIZE];
    char fileBuf[FILEBUFSIZE];
    int fileNameSize;
    int read_len;
    int fileSize = 0;

    if(argc != 3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        ErrorHandling("socket() error");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        ErrorHandling("connect() error");
    }

    //命令行获取fileName
    fputs("Input fileName : ", stdout);
    fgets(fileName, FILENAMESIZE, stdin);
    fileNameSize = strlen(fileName) - 1;
    fileName[fileNameSize] = 0; //去掉'\n'换行符
    if(write(sock, fileName, fileNameSize) == -1){
        ErrorHandling("write() error");
    }

    //接收fileName文件
    FILE* filestream;
    filestream = fopen("exer6_receive.txt", "w");
    if(filestream == NULL){
        ErrorHandling("fopen() error");
    }
    printf("file content : \n");
    while (1)
    {
        read_len = read(sock, fileBuf, FILEBUFSIZE - 1);
        if(read_len == -1) ErrorHandling("read() error");
        if(read_len == 0) {
            printf("\nreceive file : '%s' complete! (%d bytes)\n", fileName, fileSize);
            break;
        }
        fileBuf[read_len] = 0;
        fputs(fileBuf, stdout); //将文件内容打印在命令行
        fileSize += read_len;

        //将文件内容写入本地文件
        if(fwrite(fileBuf, 1, read_len, filestream) == EOF){
            ErrorHandling("fwrite() error");
        }
    }
    fclose(filestream);
    close(sock);
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}