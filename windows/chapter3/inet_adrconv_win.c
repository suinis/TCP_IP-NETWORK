#include <stdio.h>
#include <string.h>
#include <WinSock2.h>

void ErrorHandling(const char* message);

int main(int argc, char* argv[]){
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        ErrorHandling("WSAStartup() error");
    }

    // inet_addr函数调用示例
    {
        char* host_ptr1 = "1.2.3.4";
        char* host_ptr2 = "1.2.3.256";
        unsigned long net_addr1;
        unsigned long net_addr2;
        net_addr1 = inet_addr(host_ptr1);
        net_addr2 = inet_addr(host_ptr2);
        if(net_addr1 == INADDR_NONE){
            ErrorHandling("host_ptr1 ip is invalid!");
        }else{
            printf("networrk ip1 : %#x\n", net_addr1);
        }

        // if(net_addr2 == INADDR_NONE){
        //     ErrorHandling("host_ptr2 ip is invalid!");
        // }else{
        //     printf("networrk ip2 : %#x\n", net_addr2);
        // }
    }

    // inet_ntoa函数调用示例
    {
        char* host_addr;
        struct sockaddr_in sockaddr1, sockaddr2;
        sockaddr1.sin_addr.s_addr = htonl(0x12345678);
        sockaddr2.sin_addr.s_addr = htonl(0x11111111);
        //这样下面两次都会输出0x11111111的ip地址
        //因为inet_ntoa返回的是字符串的地址，意味着inet_ntoa在执行时申请了一片内存空间用于存储字符串，
        //后续再次执行inet_ntoa，且不立刻将前一次的结果复制到自己定义的空间，inet_ntoa就会将第二次生成的字符串覆盖到原第一次内存空间中
        //意味着host_addr1和host_addr2都指向inet_ntoa的内存空间中，而其中存放的时第二次结果
        host_addr = inet_ntoa(sockaddr1.sin_addr); //让host_addr指向inet_ntoa申请的内存空间
        if(host_addr == "-1"){
            printf("sockaddr1 is invalid");
        }else{
            printf("host1 ip is : %s\n", host_addr);
        }

        inet_ntoa(sockaddr2.sin_addr); //inet_ntoa内存中的字符串值被覆盖为新的
        if(host_addr == "-1"){
            printf("sockaddr2 is invalid");
        }else{
            printf("host2 ip is : %s\n", host_addr);
        }
    }
    WSACleanup();
    return 0;
}

void ErrorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}