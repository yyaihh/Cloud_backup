#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
int main(){
    pid_t pid = fork();
    if(pid == 0){
        sleep(30);
        _exit(0);
    }
    wait(NULL);
    return 0;
}
