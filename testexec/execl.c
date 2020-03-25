#include<stdio.h>
#include<unistd.h>
int main(){
    execl("/bin/ls", "ls", "/", NULL);
    printf("hello world!\n");
    return 0;
}
