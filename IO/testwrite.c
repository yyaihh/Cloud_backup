#include<stdio.h>
#include<unistd.h>
int main(){
    //printf("hellon");
    write(2, "hello", 5);
    _exit(0);
}
