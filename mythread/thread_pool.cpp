#include<iostream>
#include<cstdio>
#include<pthread.h>
#include"thread_pool.hpp"
void test(int data){
    printf("thread:%p get data:%d\n", pthread_self(), data);
}
int main(int argc, char* argv[]){
    if(argc != 2){
        return -1;
    }
    ThreadPool pool(4);
    for(int i = 0; i < atoi(argv[1]); ++i){
        pool.PushTask(ThreadTask(i, test));
    }
    return 0;
}
