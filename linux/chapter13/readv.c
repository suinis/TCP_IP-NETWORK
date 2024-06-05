#include <stdio.h>
#include <sys/uio.h>

#define BUFSIZE 1024

int main(){
    struct iovec vec[2];
    char buf1[BUFSIZE] = {0,};
    char buf2[BUFSIZE] = {0,};
    int str_len;
    vec[0].iov_base = buf1;
    vec[0].iov_len = 5; //这里设置的长度不能大于数组定义时的长度，否则输出时会错误输出控制台输入的全部信息
    vec[1].iov_base = buf2;
    vec[1].iov_len = BUFSIZE;

    str_len = readv(0, vec, 2);
    printf("Read bytes : %d \n", str_len);
    printf("First message : %s \n", buf1);
    //printf 使用 %s 格式符来打印字符串，这需要一个以 null 字符 \0 结尾的字符串。
    //如果 buf1 没有足够的空间来包括这个 null 字符（在这种情况下是输入超过或等于数组大小），printf 将继续读取内存直到遇到一个 \0 字符。
    printf("Second message : %s \n", buf2);
    return 0;
}