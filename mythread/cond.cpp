#include<iostream>
#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<pthread.h>
using namespace std;

int cate = 0;
pthread_mutex_t mutex;
pthread_cond_t foodie_cond;
pthread_cond_t chief_cond;
void* foodie(void* arg){//吃货线程
    int n = *(int*)arg;
    while(1){
        pthread_mutex_lock(&mutex);
        while(cate == 0){//没有美食只能等等着厨师做好之后叫我了
            pthread_cond_wait(&foodie_cond, &mutex);
        }
        --cate;
        cout << "吃货" << n <<":有好吃的, 马上吃一碗\n";
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&chief_cond);
    }
    return NULL;
}
void* chief(void* arg){//厨师线程
    int n = *(int*)arg;
    while(1){
        pthread_mutex_lock(&mutex);
        while(cate == 1){
            pthread_cond_wait(&chief_cond, &mutex);
        }
        ++cate;//做一份美食, 唤醒一下吃货, 别睡了, 起来吃好吃的啦
        cout << "大厨" << n <<":本大厨做了一份美食\n";
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&foodie_cond);
    }
}
int main(){
    pthread_t foodie_tid[5], chief_tid[5];
    int n[5] = {1, 2, 3, 4, 5};
    int ret;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&foodie_cond, NULL);
    pthread_cond_init(&chief_cond, NULL);
    for(int i = 0; i < 5; ++i){
        ret = pthread_create(&foodie_tid[i], NULL, foodie, (void*)(n + i));
        if(ret){
            fprintf(stderr, "foodie_thread%d create:%s\n", i + 1, strerror(ret));
            return -1;
        }
    
    }
    for(int i = 0; i < 5; ++i){
        ret = pthread_create(&chief_tid[i], NULL, chief, (void*)(n + i));
        if(ret){
            fprintf(stderr, "chief_thread%d create:%s\n", i + 1, strerror(ret));
            return -1;
        }
    }
    for(int i = 0; i < 5; pthread_join(foodie_tid[i], NULL), ++i);
    for(int i = 0; i < 5; pthread_join(chief_tid[i], NULL), ++i);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&foodie_cond);
    pthread_cond_destroy(&chief_cond);
    return 0;
}
