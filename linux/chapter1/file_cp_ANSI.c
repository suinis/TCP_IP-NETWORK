#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
    FILE *src_file, *dst_file;
    char buf[BUFSIZE];
    size_t bytes_read, bytes_write; 
    memset(buf, 0, BUFSIZE);

    if(argc != 3){
        printf("Usage:%s <src_filepath> <dst_filepath>\n", argv[0]);
        exit(1);
    }
    
    if((src_file = fopen(argv[1], "r")) == NULL){
        ErrorHandling("src_file fopen() error");
    }

    if((dst_file = fopen(argv[2], "w+")) == NULL){
        fclose(src_file);
        ErrorHandling("dst_file fopen() error");
    }

    while ((bytes_read = fread(buf, 1, BUFSIZE, src_file)) > 0)
    {
        bytes_write = fwrite(buf, 1 ,bytes_read, dst_file);
        if(bytes_read != bytes_write){
            fclose(src_file);
            fclose(dst_file);
            ErrorHandling("fwrite() error");
        }
    }

        // 检查是否到达文件末尾
    if (feof(src_file)) {
        fclose(src_file);
        fclose(dst_file);
        printf("File copied successfully!\n");
    } else if (ferror(src_file)) { // 检查是否有读取错误
        fclose(src_file);
        fclose(dst_file);
        ErrorHandling("fread() error");
    }
    
    // if(bytes_read == EOF){
    //     fclose(src_file);
    //     fclose(dst_file);
    //     ErrorHandling("fread() error");
    // }
    // fclose(src_file);
    // fclose(dst_file);
    // printf("File copied successfully!\n");

    return 0;
}

void ErrorHandling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}