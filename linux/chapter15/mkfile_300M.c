#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *file;
    const char *filename = "large_text_file.txt";
    const long size_in_mb = 300;
    const long chars_per_mb = 1024 * 1024; // 每MB约有1048576个字符
    long total_chars = chars_per_mb * size_in_mb;
    const char *text = "这里是一些文本，用于填充文件。\n";
    int text_length = strlen(text);

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法打开文件 %s\n", filename);
        return 1;
    }

    for (long i = 0; i < total_chars / text_length; i++) {
        fputs(text, file);
    }

    fclose(file);
    printf("文件 %s 已创建，大小约为 %ld MB\n", filename, size_in_mb);

    return 0;
}
