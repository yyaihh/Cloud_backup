#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc, char* argv[]){
    char* envp[] = {"PATH=/home/test", NULL};
    execve("/bin/env", argv, envp);
    return 0;
}
