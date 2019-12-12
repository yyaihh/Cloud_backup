#include<stdio.h>
#include<fcntl.h>
int main(){
    close(0);
    close(2);
    int fd1 = open("myfile.txt", O_RDONLY);
    int fd2 = open("myfile2.txt", O_RDONLY);
    if(fd1 < 0 && fd2 < 0){
        perror("open");
        return -1;
    }
    printf("fd1:%d\n", fd1);
    printf("fd2:%d\n", fd2);
    close(fd1);
    close(fd2);
    return 0;
}
