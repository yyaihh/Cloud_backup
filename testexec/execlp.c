#include<stdio.h>
#include<unistd.h>
int main(int argc, char* argv[]){
    char* arg[] = {"ls","-a","-l",NULL };//参数数组
    execlp("ls", "-a", "-l", NULL);
    printf("hello world!\n");
    return 0;
}
