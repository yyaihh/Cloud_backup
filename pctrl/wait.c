#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error");
        exit(-1);
    }else if (pid == 0) {
        sleep(5);
        exit(99);
    }
    int status = -1;
    //wait(NULL);
    while(waitpid(-1, &status, WNOHANG) == 0) {
        printf("坐一会，再来看一眼\n");
        sleep(1);
    }
    if (WIFEXITED(status)) {
         printf("child exit retval:%d\n", WEXITSTATUS(status));
    }

    if (!(status & 0x7f)) {
         printf("child exit retval:%d\n", (status >> 8)  & 0xff);
    }else {
        printf("process occured exception\n");
    }

    int ret = waitpid(pid, NULL, WNOHANG);
    if (ret < 0) {
        perror("waitpid error");
    }else if (ret == 0){
        printf("have no child exit\n");
    }else {
        printf("pid:%d child exited\n", ret);
    }
    while(1) {
        printf("打麻将~~~\n");
        sleep(1);
    }
    return 0;
}
