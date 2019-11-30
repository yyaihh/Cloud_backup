#include<stdio.h>
#include<unistd.h>
int main(){
    pid_t pid = fork();
    if(pid == -1){
        perror("fork erro\n");
    }
    else if(pid > 0){
        _exit(0);
    }
    else{
        sleep(2);
        printf("cc");
        _exit(0);
    }
    return 0;
}
