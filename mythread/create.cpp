#include<iostream>
#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<pthread.h>
void* thread_start(void* arg){
    while(1){
        printf("主线程向我传递了一个参数:%s\n", (char*)arg);
        sleep(1);
    }
    return NULL;
}
int main(){
    pthread_t tid;
    char buf[] = "哈哈哈哈";
    int ret = pthread_create(&tid, NULL, thread_start, (void*)buf);
    if(ret){
        fprintf(stderr, "pthread_create: %s\n", strerror(ret));//strerror函数
        //perror("pthread_create:");//如果置全局的errno, 则可以直接用perror, 
        //但pthread_create并不会改变全局的errno, 而是将errno返回
        return -1;
    }
    while(1){
        printf("我是主线程\n");
        sleep(1);
    }
    return 0;
}
