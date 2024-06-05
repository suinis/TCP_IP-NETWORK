#include <stdio.h>
#include <fcntl.h>  //O_RDONLY  .etc

#define BUFSIZE 3

int main(){
    int fd1, fd2;
    char buf[BUFSIZE];
    int read_len;

    fd1 = open("large_text_file.txt", O_RDONLY);
    fd2 = open("cpy.txt", O_WRONLY | O_CREAT | O_TRUNC);

    while ((read_len = read(fd1, buf, sizeof(buf))) > 0)
    {
        write(fd2, buf, read_len);
    }

    puts("copy file finished!");

    close(fd1);
    close(fd2);
    
    return 0;
}