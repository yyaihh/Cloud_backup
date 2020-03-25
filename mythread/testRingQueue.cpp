#include<iostream>
#include<cstdio>
#include<cstring>
#include<pthread.h>
#include"RingQueue.h"

#define MAX_THR 5
void* producer(void* arg){
    RingQueue<int>* p = (RingQueue<int>*)arg;
    while(1){
        p->Push(1);
       // printf("生产线程:%p: 我生产了一份数据\n", pthread_self());
    }
    return NULL;
}

void* consumer(void* arg){ 
    RingQueue<int>* p = (RingQueue<int>*)arg;
    int data;
    while(1){
        p->Pop(&data);
        //printf("消费者线程%p: 我拿到了一份数据\n", pthread_self());
    }
    return NULL;
}

int main(){
    RingQueue<int> rq;
    pthread_t con_tid[MAX_THR], pro_tid[MAX_THR];
    int ret;
    for(int i = 0; i < MAX_THR; ++i){
        ret = pthread_create(&pro_tid[i], NULL, producer, (void*)&rq);
        if(ret){
            fprintf(stderr, "producer thread%d create:%s\n", i + 1,  strerror(ret));
            return -1;
        }
    }
    for(int i = 0; i < MAX_THR; ++i){
        ret = pthread_create(&con_tid[i], NULL, consumer, (void*)&rq);
        if(ret){
            fprintf(stderr, "consumer thread%d create:%s\n", i + 1,  strerror(ret));
            return -1;
        }
    }
    for(int i = 0; i < MAX_THR; pthread_join(con_tid[i], NULL), ++i);
    for(int i = 0; i < MAX_THR; pthread_join(pro_tid[i], NULL), ++i);
    return 0;
}
