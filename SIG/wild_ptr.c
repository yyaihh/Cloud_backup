#include<stdio.h>
#include<signal.h>
#include<unistd.h>
void hander(int sig){
    printf("段错误(吐核)\n");
    printf("捕捉到的信号为 %d 号\n", sig);
    raise(SIGKILL);//raise(9);
}
int main(){
    signal(SIGSEGV, hander);
    int * p = NULL;
    *p = 10;
    return 0;
}
