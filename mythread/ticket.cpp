#include<iostream>
#include<unistd.h>
#include<pthread.h>
using namespace std;
int ticket = 100;
void* route(void *arg){
    int n = *(int*)arg;
    while(1){
        if(ticket > 0){
            usleep(1000);
            cout << "第"<< n <<"个黄牛抢到票, 还剩"<< ticket << endl;
            --ticket;
        }
        else{
            break;
        }
    }
    return NULL;
}
int main(){
    pthread_t tid[4];
    for(int i = 0; i < 4; ++i){
        int ret = pthread_create(tid + i, NULL, route, &i);
        if(ret > 0){
            cout << "创建进程失败\n";
        }
    }
    return 0;
}
