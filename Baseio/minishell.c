#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/wait.h>

int main()
{
    while(1) {
        printf("[username@localhost]$ ");
        fflush(stdout);
        char cmd_buf[1024] = {0};
        fgets(cmd_buf, 1023, stdin);//从标准输入读取数据
        cmd_buf[strlen(cmd_buf) - 1] = '\0';
        printf("cmd:[%s]\n", cmd_buf);

        //ls -l >> a.txt   
        char *ptr = cmd_buf;
        char *filename = NULL;
        int redirect_flag = 0;
        while(*ptr != '\0') {
            if (*ptr == '>') {
                *ptr = '\0';
                redirect_flag++;
                ptr++;
                if (*ptr == '>') {
                    redirect_flag++;
                    ptr++;
                }
                while(isspace(*ptr) && *ptr != '\0') ptr++;
                filename = ptr;
                while(!isspace(*ptr) && *ptr != '\0') ptr++;
                *ptr = '\0';
                continue;
            }
            ptr++;
        }
        

        char *argv[32] = {NULL};
        int argc = 0;
        ptr = cmd_buf;
        while(*ptr != '\0') {
            if (!isspace(*ptr)) {
                argv[argc] = ptr;
                argc++;
                while(!isspace(*ptr) && *ptr != '\0') ptr++;
                *ptr = '\0';
            }
            ptr++;
        }
        argv[argc] = NULL;

        pid_t pid = fork();
        if (pid < 0) {
            continue;
        }else if (pid == 0) {
            int fd = -1;
            if (redirect_flag == 1) {
                //清空重定向
                fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC,
                        0664);
                dup2(fd, 1);
            }else if (redirect_flag == 2) {
                //追加重定向
                fd = open(filename, O_CREAT|O_WRONLY|O_APPEND,
                        0664);
                dup2(fd, 1);
            }
            execvp(argv[0], argv);
            //若程序替换失败，因为子进程运行的代码和父进程一样
            //替换失败，则子进程就是另一个shell，一个终端不需要多个
            //shell，因此替换失败 ，则子进程直接退出
            exit(0);
        }
        wait(NULL);
    }
    return 0;
}
