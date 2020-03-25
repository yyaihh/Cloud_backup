#include<iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
using namespace std;
void* thread_start1(void* arg) {
    pthread_t tid = pthread_self();
    int ret = pthread_detach(tid);
    if(ret){
        fprintf(stderr, "pthread_detach thread1:%s\n", strerror(ret));
    }
    else{
        printf("thread1, id:%p已分离\n", tid);
    }
    usleep(1000);
    return NULL;
}
void* thread_start2(void* arg){
    usleep(1000);
    return NULL;
}
void* thread_start3(void* arg){
    pthread_t tid = pthread_self();
    int ret = pthread_detach(tid);
    if(ret){
        fprintf(stderr, "pthread_detach thread3:%s\n", strerror(ret));
    }
    else{
        printf("thread3, id:%p已分离\n", tid);
    }
    usleep(1000);
    return NULL;
}
int main() {
    pthread_t tid;
    //thread1
    int ret = pthread_create(&tid, NULL, thread_start1, NULL);
    if (ret){
        fprintf(stderr, "pthread_create thread1:%s\n", strerror(ret));
        return -1;
    }
    //thread2
    usleep(100);
    ret = pthread_create(&tid, NULL, thread_start2, NULL);
    if (ret){
        fprintf(stderr, "pthread_create thread2:%s\n", strerror(ret));
        return -1;
    }
    ret = pthread_detach(tid);
    if(ret){
        fprintf(stderr, "pthread_detach thread2:%s\n", strerror(ret));
    }
    else{
        printf("thread2, id:%p已分离\n", tid);
    }
    //thread3
    ret = pthread_create(&tid, NULL, thread_start3, nullptr);
    if (ret){
        fprintf(stderr, "pthread_create thread3:%s\n", strerror(ret));
        return -1;
    }
    usleep(1000);//必须要睡一下,要让线程先分离，再等待
    //线程被创建, 但创建的线程中的分离语句和下面的join语句
    //哪个先执行还真不一定, 所以说要先睡一下
    ret = pthread_join(tid, NULL);
    if (ret == 0) {
        printf("pthread3 wait success\n");

    } else {
        fprintf(stderr, "pthread3 wait failed:%s\n", strerror(ret));
    }
    return 0;
}
