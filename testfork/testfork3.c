#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(){
    pid_t pid = fork();
    int data = 0;
    if(pid == -1){
        perror("fork erro");
    }
    else if(pid == 0){
        printf("子进程执行\n");
        data = 10;
        printf("data = %d\n", data);
        printf("data地址: %p\n", &data);
    }
    else{
        sleep(2);
        printf("父进程执行\n");
        printf("data = %d\n", data);
        printf("data地址: %p\n", &data);
    }
    return 0;
}
