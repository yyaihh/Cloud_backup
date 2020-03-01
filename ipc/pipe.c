#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
int main(){
    int fd[2];
    char buf[100];
    int len;
    if(pipe(fd) < 0){
        perror("make pipe");
        exit(1);
    }
    while(fgets(buf, 100, stdin)){
        len = strlen(buf);
        if(write(fd[1], buf, len) != len){
            perror("write to pipe");
            break;
        }
        memset(buf, 0, sizeof(buf));
        if((len = read(fd[0], buf, 100)) == -1){
            perror("read from pipe");
            break;
        }
        if(write(1, buf, len) != len){
            perror("write to stdout");
            break;
        }
    }
    exit(0);
}
