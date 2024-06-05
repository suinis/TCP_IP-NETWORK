#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

#define BUFSIZE 30

// struct ip_mreq
// {
//     /* IP multicast address of group.  */
//     struct in_addr imr_multiaddr;

//     /* Local IP address of interface.  */
//     struct in_addr imr_interface;
// };

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    int recv_sock;
    struct sockaddr_in recv_adr;
    struct ip_mreq join_adr;
    int recv_len;
    char buf[BUFSIZE];

    int i = 0;

    if(argc != 3){
        printf("Usage : %s <GROUPIP> <PORT> \n", argv[0]);
        exit(1);
    }

    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(recv_sock == -1) 
        ErrorHandling("socket() error");

    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi(argv[2]));

    if(bind(recv_sock, (struct sockaddr*)& recv_adr, sizeof(recv_adr)) == -1)
        ErrorHandling("bind() error");

    join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]); //要加入的组的IP地址
    join_adr.imr_interface.s_addr = htonl(INADDR_ANY); //加入组的套接字所属主机的IP地址

    //加入join_adr广播组
    if(setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)& join_adr, sizeof(join_adr)) == -1)
        ErrorHandling("setsockopt() error");

    while(1){
        recv_len = recvfrom(recv_sock, buf, BUFSIZE - 1, 0, NULL, 0); //如果不需要知道传输数据的主机地址信息，第五个参数设为：NULL，第六个参数设为：0
        if(recv_len < 0)
            break;
        buf[recv_len] = 0;
        // fputs(buf, stdout);
        printf("message%d : %s \n", ++i, buf);
    }
    close(recv_sock);

    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}