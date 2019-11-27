/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述：通过这个demo认识匿名管道的基本操作，并且体会管道的特性 
================================================================*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int pipefd[2] = {0};
    int ret = pipe(pipefd);
    if (ret < 0) {
        perror("pipe error");
        return -1;
    }
    int pid = fork();
    if (pid < 0) {
        perror("fork error");
        return -1;
    }else if (pid == 0) {
        //子进程
        close(pipefd[0]);
        pause();


        char buf[1024] = {0};
        int ret = read(pipefd[0], buf, 1023);
        printf("buf:[%s]-[%d]\n", buf, ret);
    }else {
        //父进程
        sleep(1);
        close(pipefd[0]);



        char *ptr = "今天天气好晴朗~~";
        int total = 0;
        int ret = write(pipefd[1], ptr, strlen(ptr));
        total += ret;
        printf("total:%d\n", total);
    }
    /*
    while(1) {
        printf("---------%d\n", getpid());
        sleep(1);
    }
    */
    return 0;
}
