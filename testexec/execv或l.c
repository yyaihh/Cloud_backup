#include<stdio.h>
#include<unistd.h>
int main(int argc, char* argv[]){
    char *env[3] = { NULL };
    env[0] = "MYVAL=1000";
    char* arg[3] = {NULL};
    arg[0] = "myenv";
    arg[1] = "-i";
    arg[2] = NULL;
    execv("./myenv", arg);
    printf("hello world!\n");
    return 0;
}
