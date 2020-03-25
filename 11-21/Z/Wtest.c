#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include<sys/wait.h>
int main() {
    pid_t pid = fork();
    if(pid < 0){ 
        perror("fork"); 
        return 1; 
    }
    else if(pid == 0){
        sleep(5); 
        exit(EXIT_SUCCESS);  
    }
    printf("父进程被wait()阻塞, 等待子进程退出并回收\n");
    wait(NULL);
    return 0; 
}
