#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main() {
    printf("Before fork\n");
    pid_t pid = fork();
    
    if (pid == 0) {
        printf("Child process\n");
    } else {
        printf("Parent process\n");
    }

    printf("After fork\n");
    return 0;
}
