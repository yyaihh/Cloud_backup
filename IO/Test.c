#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main(){
    fwrite("fwrite()  ", 1, 10, stdout);
    write(2, "write()  ", 9);
    _exit(0);
}
