#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
int main(){
    printf("一位出租车司机在车站拉乘客~\n");
    pid_t pid = fork();
    if(pid < 0){
        perror("fork erro");
        exit(-1);                  
    }
    else if(pid == 0){
        sleep(5);
        exit(5);                           
    }
    int status = -1;
    printf("%d\n", WNOHANG);
    while(waitpid(-1, &status, 1) == 0){
        printf("没有人打车, 继续等...\n");
        sleep(1);                           
    }
    if(WIFEXITED(status)){
        printf("这次等了%d分钟才拉到乘客(子进程退出码)\n", WEXITSTATUS(status));
        printf("有人打车了, 出发~\n");
    }
    else{
        printf("没有等到乘客(子进程异常退出)\n");
    }
    return 0;

}

