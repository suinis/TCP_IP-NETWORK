#include <stdio.h>
#include <fcntl.h>

int main(){
    int fd;
    FILE* fp;

    fd = open("data.dat", O_RDONLY);
    if((fp = fdopen(fd, "w")) == NULL){
        puts("fdopen() error");
    }
    return 0;
}