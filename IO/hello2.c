#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
    umask(0);
    int fd = open("myfile2.txt", O_RDWR | O_CREAT, 0644);
    if(fd < 0){
        printf("打开失败\n");
        return 0;
    }
    const char* str = "hello world!\n";
    int count = 5;
    int len = strlen(str);
    while(count--){
        write(fd, str, len);
    }
    lseek(fd, SEEK_SET, 0);
    char buf[1024] = { 0 };
    count = 5;
    size_t n;
    while(count--){
        n = read(fd, buf, len);
        if(n > 0){
            printf("%s", buf);
        }
        else{
            break;
        }
    }
    close(fd);
    return 0;
}
