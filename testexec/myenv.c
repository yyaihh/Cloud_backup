#include<stdio.h>
#include<unistd.h>
int main(int argc, char *argv[], char *env[]) {
    int i = 0;
    for (; i < argc; i++) {
        printf("argv[%d]=[%s]\n", i, argv[i]);
    }
    return 0;
}

