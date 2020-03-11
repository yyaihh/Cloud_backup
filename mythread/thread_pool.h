
#include<iostream>
#include<pthread.h>
#include<cstdio>
#include<cstring>
#include<queue>
using namespace std;

typedef void (*task_handler_t)(int);
class ThreadTask{
    int m_data;
    task_handler_t m_handler;
public:
    ThreadTask(int data, task_handler_t handler) :
        m_data(data), m_handler(handler)
    {}
    void Run(){
        return m_handler(m_data);
    } 
};
class ThreadPool{
    int m_max_count;//定义线程池中线程的数量
    queue<ThreadTask> m_task_q;
    pthread_mutex_t m_mutex;
    pthread_cond_t cond;
    void* thr_start(void* arg){

    }
public:
    bool PushTask(const ThreadTask& task){

        return true;
    }
};
