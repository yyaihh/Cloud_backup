#include<iostream>
#include<cstdio>
#include<unistd.h>
#include<pthread.h>
#include"thread_pool.h"
pthread_mutex_t mutex;
void test(int data){
    pthread_mutex_lock(&mutex);
    printf("thread:%p get data:%d\n", pthread_self(), data);
    //sleep(1);
    pthread_mutex_unlock(&mutex);
}
int main(){
    ThreadPool pool(4);
    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i < 10; ++i){
        pool.PushTask(ThreadTask(i, test));
    }
    pthread_mutex_destroy(&mutex);
    sleep(3);
    pool.~ThreadPool();
    sleep(2);
    return 0;
}
