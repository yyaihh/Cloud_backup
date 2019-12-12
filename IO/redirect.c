#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
int main(){
    close(1);
    int fd = open("redirect.txt", O_WRONLY | O_CREAT, 0644);
    if(fd < 0){
        perror("open");
        return -1;
    }
    printf("fd:%d\n", fd);
    //fflush(stdout);//刷新输出缓冲区
    return 0;
}
