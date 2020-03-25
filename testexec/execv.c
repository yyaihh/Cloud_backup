#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc, char* argv[]){
    char* arg[4] = {"ls","-a","-l",NULL };//参数数组
    pid_t pid = fork();
    if(pid == -1){
        perror("fork");
    }
    else if(pid == 0){
        printf("替换子进程\n");
        execv("/bin/ls", argv);
    }
    else{
        sleep(1);
        printf("替换父进程\n");
        execv("/bin/ls", arg);
    }
    printf("hello world!\n");
    return 0;
}
