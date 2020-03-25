#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main(){
    printf("******* %d\n", getpid());
    pid_t pid = fork();
    if(pid < 0){
        printf("fork error\n");
        return -1;
    }
    else if(pid == 0){
        printf("child  %d\n", getpid());
        //这段代码只有子进程能够运行到, 因为在子进程中fork的返回值=0
        sleep(5);
        exit(0);
    }
    else{
        //只有父进程才能运行到
        printf("parent  %d\n", getpid());
    }
    while(1){
        printf("hello world\n");
        sleep(1);
    }
    return 0;
}
