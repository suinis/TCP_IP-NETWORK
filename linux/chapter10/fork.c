#include <stdio.h>
#include <unistd.h>

int gval = 10;
int main(int argc, char* argv[]){
    __pid_t pid;
    int lval = 20;
    gval++, lval += 5; //gval = 11, lval = 25

    pid = fork();
    if(pid == 0){
        gval += 2;
        lval += 2;
    }else{
        gval -= 2;
        lval -= 2;
    }

    if(pid == 0) printf("Child Proc : gval = %d, lval = %d\n", gval, lval);
    else printf("Parent Proc : gval = %d, lval = %d\n", gval, lval);

    if(pid == 0) printf("Child Proc finished!\n");
    else printf("Parent Proc finished!\n");
    gval = 100; //判断子进程复制内存从何时开始。如果是在父进程之后进行，子进程是应该复制gval = 100。实际上是9 += 2 -> 13，所以假设×

    return 0;
}