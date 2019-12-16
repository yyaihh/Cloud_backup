#include<stdio.h>
#include<unistd.h>

int main(){
    int pid = fork();
    if(pid<0){
        perror("fork erro\n");
    }
    else if(pid == 0){
        printf("child\n");
    }
    else{
        printf("prents\n");
    }
    return 0;
}
