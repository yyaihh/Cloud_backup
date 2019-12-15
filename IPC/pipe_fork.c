#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
int main(){
    int pfd[2];
    if(pipe(pfd) < 0){
        perror("make pipe");
        exit(1);
    }
    pid_t pid = fork();
    if(pid < 0){
        perror("fork");
    }
    else if(pid == 0){
        char buf[100];
        int len, n = 3;
        close(pfd[0]);
        while(n-- && fgets(buf, 100, stdin)){
            len = strlen(buf);
            printf("子进程将所输字符串写入管道\n");
            if(write(pfd[1], buf, len) != len){
                perror("write to pipe");
            } 
            usleep(100);
        }
        close(pfd[1]);
    }
    else{
        char buf[100];
        int len, n = 3;
        close(pfd[1]);
        while(n--){
            usleep(100);
            if((len = read(pfd[0], buf, 100)) == -1){
               perror("read from pipe");
            }  
            write(1, buf, len);
            printf("父进程从管道读出字符串, 打印到屏幕\n");
        }
        close(pfd[0]);
    }
    exit(0);
}
