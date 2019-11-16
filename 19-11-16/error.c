#include<stdio.h>
#include<unistd.h>
#include<string.h>
int main(){
    int i;
    for(i = 0;i<256;++i){
        printf("[%d] %s\n",i,strerror(i));
    }
    return 0;
}
