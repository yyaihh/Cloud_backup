#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main(){
    char* envp[] = {"PATH=/home/test", NULL};
    execle("/bin/env", "", NULL, envp);
    return 0;
}
