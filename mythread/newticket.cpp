#include<iostream>
#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<pthread.h>
using namespace std;

int ticket = 30;
pthread_mutex_t mutex;
void* route(void *arg){
    while(1){
        pthread_mutex_lock(&mutex);
        if(ticket > 0){
            usleep(100);
            --ticket;
            printf("第%d位黄牛抢到票, 还剩%d张\n", *(int*)arg, ticket);
            pthread_mutex_unlock(&mutex);
            usleep(10);
        }
        else{
            pthread_mutex_unlock(&mutex);
            break;
        }
    }
    return NULL;
}
int main(){
    pthread_t tid[5];
    int n[5] = {1,2,3,4,5};
    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i < 5; ++i){
        int ret = pthread_create(&tid[i], NULL, route, n + i);
        if(ret){
            fprintf(stderr, "pthread_create:%s\n", strerror(ret));
        }
    }
    for(int i = 0; i < 5; ++i){
        pthread_join(tid[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
