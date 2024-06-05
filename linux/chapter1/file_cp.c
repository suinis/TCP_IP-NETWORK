#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE 1024

void ErrorHandling(char* message);

int main(int argc, char* argv[]){
    size_t src_fd;
    size_t dst_fd;
    char buf[BUFSIZE];
    ssize_t bytes_read, bytes_write;
    if(argc != 3){
        printf("Usage:%s <src_filepath> <dst_filepath>\n", argv[0]);
        exit(1);
    }

    src_fd = open(argv[1], O_RDONLY);
    if(src_fd == -1){
        ErrorHandling("src_file open() error");
    }

    dst_fd = open(argv[2], O_CREAT|O_TRUNC|O_WRONLY, 0644);
    if(dst_fd == -1){
        close(src_fd);
        ErrorHandling("dst_file open() error");
    }

    while((bytes_read = read(src_fd, buf, BUFSIZE)) > 0){
        bytes_write = write(dst_fd, buf, bytes_read);
        if(bytes_write != bytes_read){
            close(src_fd);
            close(dst_fd);
            ErrorHandling("write() error");
        }
    }

    if(bytes_read == -1){
        close(src_fd);
        close(dst_fd);
        ErrorHandling("read() error");
    }

    close(src_fd);
    close(dst_fd);
    printf("File copied successfully!\n");

    return 0;
}

void ErrorHandling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}