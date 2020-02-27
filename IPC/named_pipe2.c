#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
int main(){
    int write_fd = open("my_named_pipe", O_WRONLY);
    if(write_fd < 0){
        perror("open");
    }
    char buf[1024] = {0};
    ssize_t s;
    for(int i = 0; i < 5; ++i){
        printf("Please Enter# ");
        fflush(stdout);
        s = read(0, buf, 1023);
        if(s > 0){
            buf[s] = '\0';
            write(write_fd, buf, strlen(buf));
        }
        else{
            perror("read");
        }
    }
        close(write_fd);
    return 0;
}
