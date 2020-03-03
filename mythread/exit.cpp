#include<iostream>
#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<pthread.h>
using namespace std;
int n = 2;
void* thread_start1(void* arg){
    int* p = new int;
    *p = 1;
    return (void*)p;
}
void* thread_start2(void* arg){
    int *p = &n;
    pthread_exit((void*)p);
}
void* thread_start3(void* arg){
    while(1){
        sleep(1);
        printf("thread3 is runing ...\n");
    }
    return NULL;
}

int main(){
    pthread_t tid[3];
    void* (*thread_start[3])(void*) = {thread_start1, thread_start2, thread_start3};
    int ret;
    for(int i = 0; i < 3; ++i){
        ret = pthread_create(&tid[i], NULL, thread_start[i], NULL);
        if(ret){
            fprintf(stderr, "pthread_create%d: %s\n", i + 1, strerror(ret));
            return -1;
        }
    }
    int* retval;
    pthread_join(tid[0], (void**)&retval);
    printf("thread%d返回值为%d\n", 1, *retval);
    delete retval;
    pthread_join(tid[1], (void**)&retval);
    printf("thread%d返回值为%d\n", 2, *retval);
    sleep(3);
    pthread_cancel(tid[2]);
    pthread_join(tid[2], (void**)&retval);
    if(retval == PTHREAD_CANCELED){
        printf("thread3 return code:PTHREAD_CANCELED\n");
    }
    else{
        printf("thread%d返回值为%d\n", 2, *retval);
    }
    return 0;
}
