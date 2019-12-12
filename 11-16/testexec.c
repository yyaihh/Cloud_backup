#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(int argc, char *env[]){
    int i = 0;
    for(; i<argc;++i){
        printf("%s\n", env[i]);
    }
    return 0;
}
