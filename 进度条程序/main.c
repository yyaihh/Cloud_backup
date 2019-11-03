#include<stdio.h>
#include<unistd.h>
int main(){
    char buf[50]={0};
    const char* lable="|/-\\";
    int i = 0;
    while(i<50){
        printf("%s\r%c",buf,lable[i%4]);
        buf[i++]='#';
        fflush(stdout);
        usleep(100000);
    }
    putchar('\n');
    return 0;
}
