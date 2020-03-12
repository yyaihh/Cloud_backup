#include<iostream>
#include<cstdio>
#include<unistd.h>
#include"thread_pool.h"
void test(int data){
    printf("thread:%p get data:%d\n", pthread_self(), data);
    sleep(1);
}
int main(){
    ThreadPool pool(4);
    for(int i = 0; i < 10; ++i){
        pool.PushTask(ThreadTask(i, test));
    }
    while(1){
        sleep(1);
        //为了防止主线程先退出而看不见测验结果
    }
    return 0;
}
