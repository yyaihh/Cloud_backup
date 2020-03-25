#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
//#include<sys/types.h>
#include<stdlib.h>
int main(){
    printf("一位爸爸来学校接孩子放学了~\n");
    pid_t pid = fork();
    if(pid < 0){
        perror("fork erro");
        exit(-1);
    }
    else if(pid == 0){
        sleep(30);
        exit(30);
    }
    int status = -1;
    wait(&status);
    if (!(status & 0x7f)) {
        printf("%d分钟后接上了孩子\n", (status >> 8) & 0xff);                
    }
    else {
        printf("孩子在幼儿园发烧送医院了!\n");            
    }
    return 0;
}
