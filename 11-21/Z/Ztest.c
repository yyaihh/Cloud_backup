#include <stdio.h>
#include <stdlib.h> 
#include<unistd.h>
int main() {
    pid_t pid = fork();
    if(pid < 0){ 
        perror("fork"); 
        return 1; 
    }
    else if(pid == 0){
        printf("child[%d] is begin Z...\n", getpid());
        sleep(5); 
        exit(EXIT_SUCCESS);  
    }
    else{
        printf("parent[%d] is sleeping...\n", getpid());
        sleep(10); 
    }
    return 0; 
}
