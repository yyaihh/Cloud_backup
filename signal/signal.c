#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sigcb(int signum)
{
    printf("recv signum:%d\n", signum);
}
int main()
{
    //signal修改信号的处理方式
    //  SIG_IGN 忽略处理    SIG_DFL 默认处理方式
    //signal(SIGINT, SIG_IGN);
    signal(SIGINT, sigcb);
    signal(SIGQUIT, sigcb);
    while(1) {
        printf("hello\n");
        sleep(1);
    }
    return 0;
}
