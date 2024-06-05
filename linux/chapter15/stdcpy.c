#include <stdio.h>

#define BUFSIZE 1024

int main() {
    FILE *fp1, *fp2;
    char buf[BUFSIZE];
    
    // 打开源文件以读取
    fp1 = fopen("large_text_file.txt", "r");
    if (fp1 == NULL) {
        perror("打开源文件失败");
        return 1;  // 添加返回，确保不继续执行
    }

    // 打开目标文件以写入
    fp2 = fopen("cpy.txt", "w");
    if (fp2 == NULL) {
        perror("打开目标文件失败");
        fclose(fp1);  // 关闭已打开的源文件
        return 1;  // 添加返回，确保不继续执行
    }

    // 从源文件复制数据到目标文件
    while (fgets(buf, sizeof(buf), fp1) != NULL) {
        fputs(buf, fp2);
    }

    printf("文件复制完成!\n");

    // 关闭两个文件
    fclose(fp1);
    fclose(fp2);

    return 0;
}
