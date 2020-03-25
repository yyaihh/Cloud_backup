#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

int main()
{
    //umask设置文件的创建权限掩码，决定了文件的默认权限
    //给定权限 & ~掩码
    umask(0);
    //有可能新建文件，则一定要指定文件权限
    int fd = open("./test.txt", O_RDWR | O_CREAT, 0777);
    if (fd < 0) {
        perror("open error");
        return -1;
    }

    char *ptr = "天已经黑了~~\n";
    int ret = write(fd, ptr, strlen(ptr));
    if (ret < 0) {
        perror("write error");
        return -1;
    }
    printf("write len:%d\n", ret);

    //在读取数据之前讲文件读写位置跳转到文件起始位置
    lseek(fd, 0, SEEK_SET);

    char buf[1024] = {0};
    ret = read(fd, buf, sizeof(buf) - 1);
    if (ret < 0) {//读取数据出错
        perror("read error");
        return -1;
    }else if (ret == 0) {//读取数据到达文件末尾
        printf("at end of file\n");
        return -1;
    }
    printf("read buf:[%s]-[%d]\n", buf, ret);

    //关闭文件，释放资源
    close(fd);
    return 0;
}
