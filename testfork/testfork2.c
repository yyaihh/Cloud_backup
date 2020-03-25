#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main(){
    pid_t pid = vfork();
    if(pid == -1){
        perror("fork error");
    }
    else if(pid == 0){
        printf("子进程执行\n子进程pid:%d\n", getpid());
        exit(0);
    }
    else{
        printf("父进程开始执行\n");
        sleep(5);
        printf("父进程执行\n父进程pid:%d\n", getpid());
        printf("父进程运行结束\n");
    }
    return 0;
}
