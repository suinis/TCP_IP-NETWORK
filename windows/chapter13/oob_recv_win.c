#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUFSIZE 50

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    WSADATA wsaData;
    int acpt_sock, recv_sock;
    SOCKADDR_IN recv_adr, send_adr;
    int send_adr_sz;
    int recv_len, recv_except_len;
    char buf[BUFSIZE];

    fd_set readfds, execptfds, readcpy, exceptcpy;
    TIMEVAL timeout;
    int result; 

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
        ErrorHandling("WSAStartup() error");

    if(argc != 2){
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(acpt_sock == INVALID_SOCKET)
        ErrorHandling("socket() error");

    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(acpt_sock, (SOCKADDR*)& recv_adr, sizeof(recv_adr)) == SOCKET_ERROR)
        ErrorHandling("bind() error");
    
    if(listen(acpt_sock, 5) == SOCKET_ERROR)
        ErrorHandling("listen() error");

    send_adr_sz = sizeof(send_adr);
    recv_sock = accept(acpt_sock, (SOCKADDR*)& send_adr, &send_adr_sz);
    if(recv_sock == INVALID_SOCKET)
        ErrorHandling("accrpt() error");

    FD_ZERO(&readfds);
    FD_ZERO(&execptfds);
    FD_SET(recv_sock, &readfds);
    FD_SET(recv_sock, &execptfds);
    while (1)
    {
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        readcpy = readfds;
        exceptcpy = execptfds;

        if((result = select(0, &readcpy, 0, &exceptcpy, &timeout)) > 0){
            if(FD_ISSET(recv_sock, &readfds)){ //I/O套接字有接收事件
                recv_len = recv(recv_sock, buf, BUFSIZE - 1, 0);
                if(recv_len == -1) 
                    ErrorHandling("recv() error");
                if(recv_len == 0){
                    closesocket(recv_sock);
                    break;
                }
                else{
                    buf[recv_len] = 0;
                    puts(buf);
                }
            }

            if(FD_ISSET(recv_sock, &execptfds)){ //I/O套接字有异常事件
                while ((recv_except_len = recv(recv_sock, buf, BUFSIZE - 1, MSG_OOB)) > 0)
                {
                    buf[recv_except_len] = 0;
                    printf("Urgent message : %s \n", buf);
                }
            }
        }
    }
    closesocket(acpt_sock);

    WSACleanup();
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}