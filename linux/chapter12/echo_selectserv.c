#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#define BUFSIZE 4

#include <sys/time.h>
#include <sys/select.h>

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int serv_sock, io_sock;
    struct sockaddr_in servaddr;
    struct sockaddr_in clntaddr;
    socklen_t clnt_addr_sz;
    char buf[BUFSIZE];
    ssize_t read_len; 
    int i;

    fd_set reads, cpy_reads;
    struct timeval timeout;
    int fd_max, fd_num;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    if((serv_sock = socket(PF_INET, SOCK_STREAM, 0))  == -1){
        ErrorHandling("socket() error");
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&servaddr, sizeof(servaddr))  == -1){
        ErrorHandling("bind() error");
    }
    
    if(listen(serv_sock, 5) == -1){
        ErrorHandling("listen() error");
    }

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;

    while (1)
    {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        if((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1){
            fputs("select() error", stderr);
            fputc('\n', stderr);
            break;
        }
        if(fd_num == 0){
            // puts("Time-out!");
            continue;
        }
        puts("return select");

        //监听到三个事件，遍历查询需要的操作
        for(int i = 0; i < fd_max + 1; ++i){
            if(FD_ISSET(i, &cpy_reads)){
                if(i == serv_sock){ //建立连接
                    clnt_addr_sz = sizeof(clntaddr);
                    io_sock = accept(serv_sock, (struct sockaddr*)& clntaddr, &clnt_addr_sz);
                    if(io_sock == -1) ErrorHandling("accept() error");
                    FD_SET(io_sock, &reads);

                    if(fd_max < io_sock) fd_max = io_sock;
                    printf("Connected client%d...\n", io_sock);
                }
                else{ //读取数据
                    read_len = read(i, buf, BUFSIZE);
                    if(read_len == 0){ //读取到EOF，说明要关闭
                        FD_CLR(i, &reads);
                        close(i);
                        printf("client%d closed!\n", i);
                    }else{
                        if(write(i, buf, read_len) == -1) ErrorHandling("write() error");
                    }
                }
            }
        }
    }
    
    close(serv_sock);
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}