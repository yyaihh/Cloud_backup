#include<iostream>
#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<pthread.h>
using namespace std;

int cate = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;
void* foodie(void* arg){//吃货线程
    while(1){
        pthread_mutex_lock(&mutex);
        if(cate == 0){//没有美食只能等等着厨师做好之后叫我了
            pthread_cond_wait(&cond, &mutex);
        }
        --cate;
        cout << "吃货:有好吃的, 马上吃一碗\n";
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
    }
    return NULL;
}
void* chief(void* arg){//厨师线程
    while(1){
        pthread_mutex_lock(&mutex);
        if(cate == 1){
            pthread_cond_wait(&cond, &mutex);
        }
        ++cate;//做一份美食, 唤醒一下吃货, 别睡了, 起来吃好吃的啦
        cout << "大厨:本大厨做了一份美食\n";
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
    }
}
int main(){
    pthread_t foodie_tid, chief_tid;
    int ret;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
        ret = pthread_create(&foodie_tid, NULL, foodie, NULL);
        if(ret){
            fprintf(stderr, "foodie_thread create:%s\n", strerror(ret));
            return -1;
        }
        ret = pthread_create(&chief_tid, NULL, chief, NULL);
        if(ret){
            fprintf(stderr, "chief_thread create:%s\n", strerror(ret));
            return -1;
        }
    
    pthread_join(foodie_tid, NULL);
    pthread_join(chief_tid, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}
