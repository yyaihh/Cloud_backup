#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
int main(){
    umask(0);
    if(mkfifo("my_named_pipe", 0664) < 0){
        perror("mkfifo");
    }
    int read_fd = open("my_named_pipe", O_RDONLY);
    if(read_fd < 0){
        perror("open");
    }
    char buf[1024] = {0};
    ssize_t s;
    for(int i = 0; i < 5; ++i){
        printf("Please wait ... \n");
        s = read(read_fd, buf, 1023);
        if(s > 0){
            buf[s - 1] = '\0';
            printf("client say# %s\n", buf);
        }
        else if(s == 0){
            printf("client quit, exit now!\n");
            exit(EXIT_SUCCESS);
        }
        else{
            perror("read");
        }
    }
        close(read_fd);
    return 0;
}
