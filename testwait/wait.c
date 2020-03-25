#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
//#include<sys/types.h>
#include<stdlib.h>
int main(){
    pid_t pid = fork();
    if(pid < 0){
        perror("fork erro");
        exit(-1);
    }
    else if(pid == 0){
        int n = 5;
        int* p = NULL;
        *p = 10;
        printf("子进程执行\n");
        sleep(n);
        exit(n);
    }
    else{
        printf("父进程执行\n");
        int status;
        int ret = wait(&status);
        printf("父进程等待%d秒后继续执行\n", (status>>8)&0XFF);
        if ( ret > 0 && ( status & 0X7F  ) == 0  ){ // 正常退出
            printf("子进程退出码:%d\n", (status>>8)&0XFF);
        } 
        else if( ret > 0  ) { // 异常退出
            printf("子进程异常退出信号量: %d\n", status&0X7F );
        }
    }
    return 0;
}
