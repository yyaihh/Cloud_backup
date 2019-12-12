#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
int main(){
    //int fd = open("testdup2", O_CREAT | O_RDWR, 0664);
    int fd = open("testdup2", O_CREAT | O_RDWR | O_APPEND, 0664);
    if(fd < 0){
        perror("open");
        return -1;
    }
    dup2(fd, 1);
    printf("我被重定向了\n");
    return 0;
}
