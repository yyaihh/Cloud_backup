/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述：这是一个程序替换的demo，
*       主要用户体会程序替换接口的基本使用，以及各个接口的区别 
*       exec函数族
================================================================*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char *env[3] = {NULL};
    env[0] = "MYVAL=1000";
    env[1] = NULL;
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]=[%s]\n", i, argv[i]);
    }
    //execl("/bin/ls", "ls", "-l", NULL);
    //execlp("myenv", "myenv", "-l", NULL);
    //execle("./myenv", "myenv", "-l", NULL, env);
    char *arg[3] = {NULL};
    arg[0] = "myenv";
    arg[1] = "-i";
    arg[2] = NULL;
    execv("./myenv", arg);
    printf("hello ~~\n");
    return 0;
}
