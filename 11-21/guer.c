#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main(){
    pid_t pid = fork();
    if(pid < 0){
        perror("fork");
        exit(-1);
    }
    else if(pid == 0){
        printf("子进程睡一会\n");
        sleep(10);
    }
    else{
        printf("父进程先走一步\n");
        exit(0);
    }
    return 0;
}
