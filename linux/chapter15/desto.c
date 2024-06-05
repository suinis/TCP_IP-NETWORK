#include <stdio.h>
#include <fcntl.h>

#define BUFSIZE 30

int main(int argc, char* argv[]){
    int fd;
    FILE* fp;
    char buf[BUFSIZE];

    // fd = open("large_text_file.txt", O_RDONLY);
    // if(fd == -1){
    //     puts("file open error");
    //     return -1;
    // }
    // fp = fdopen(fd, "r");

    // fgets(buf, BUFSIZE - 1, fp);
    // puts(buf);
    // fclose(fp);

    fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC);
    if(fd == -1){
        puts("file open error");
        return -1;
    }
    fp = fdopen(fd, "w");
    fputs("Network C programing \n", fp);
    fclose(fp);
    
    return 0;
}