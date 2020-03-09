#include<iostream>
#include<cstdio>
#include<cstring>
#include<pthread.h>
#include<unistd.h>
#include"BlockingQueue.h"
#define MAX_THR 5 

void* producer(void* arg){
    BlockingQueue<int>* q = (BlockingQueue<int>*)arg;
    while(1){
        q->Push(1);
        printf("生产线程%p: 我生产了一份数据\n", pthread_self());
    }
    return NULL;
}
void* consumer(void* arg){
    BlockingQueue<int>* q = (BlockingQueue<int>*)arg;
    int data;//从阻塞队列中拿到的数据
    while(1){
        q->Pop(&data);
        printf("消费者线程%p: 我拿到了一份数据\n", pthread_self());
    }
    return NULL;
}
int main(){
    BlockingQueue<int> q(10);
    pthread_t consumer_tid[MAX_THR], producer_tid[MAX_THR];
    int ret;
    for(int i = 0; i < MAX_THR; ++i){
        ret = pthread_create(&producer_tid[i], NULL, producer, (void*)&q);
        if(ret){
            fprintf(stderr, "producer thread%d create:%s\n", i + 1, strerror(ret));
            return -1;
        }
    }
    for(int i = 0; i < MAX_THR; ++i){
        ret = pthread_create(&consumer_tid[i], NULL, consumer, (void*)&q);
        if(ret){
            fprintf(stderr, "consumer thread%d create:%s\n", i + 1, strerror(ret));
            return -1;
        }
    }
    for(int i = 0; i < MAX_THR; pthread_join(consumer_tid[i], NULL), ++i);
    for(int i = 0; i < MAX_THR; pthread_join(producer_tid[i], NULL), ++i);
    return 0;
}
