#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
void* thread_start(void* arg){
    while(1){
        printf("主线程像我传递了一个参数:%s\n", (char*)arg);
        sleep(1);
    }
    return NULL;
}
int main(){
    pthread_t tid;
    char buf[] = "hahahhahaha";
    int ret = pthread_create(&tid, NULL, thread_start, (void*)buf);
    if(ret){
        printf("thread create error:%d\n", ret);
        return -1;
    }
    while(1){
        printf("I am main thread:%p\n", tid);
        sleep(1);
    }
    return 0;
}
