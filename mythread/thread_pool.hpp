#ifndef __SOMEFILE_H__
#define __SOMEFILE_H__
#include<iostream>
#include<pthread.h>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<queue>
using namespace std;

#define MAX_COUNT 10 
typedef void (*task_handler_t)(int);
class ThreadTask{
    public:
    int m_data;
    task_handler_t m_handler;
public:
    ThreadTask(int data, task_handler_t handler) :
        m_data(data), m_handler(handler)
    {}
    void Run(){
        m_handler(m_data);
    } 
};
class ThreadPool{
    size_t m_max_count;//定义线程池中线程的数量
    queue<ThreadTask> m_task_q;//任务队列
    pthread_mutex_t m_mutex;//互斥锁
    pthread_cond_t m_pro_cond, m_con_cond;//条件变量
    bool m_exit_flag;//析构标记
    vector<pthread_t> m_tid_array;//线程池中所有线程的tid
    size_t m_destroy_count;//销毁线程计数

    static void* thr_start(void* arg);
    void Quit();
public:
    ThreadPool(size_t max_count = MAX_COUNT); 
    ~ThreadPool();
    bool PushTask(const ThreadTask& task);
};

ThreadPool::ThreadPool(size_t max_count) :
    m_max_count(max_count),
    m_exit_flag(false),
    m_destroy_count(0),
    m_tid_array(max_count) {
    pthread_mutex_init(&m_mutex, NULL);
    pthread_cond_init(&m_con_cond, NULL);
    pthread_cond_init(&m_pro_cond, NULL);
    int ret;
    for(size_t i = 0; i < m_max_count; ++i){
        ret = pthread_create(&(m_tid_array[i]), NULL, thr_start, this);
        if(ret){
            fprintf(stderr, "create thread:%s\n", strerror(ret));
            exit(0);
        }
        //若不关心线程返回值, 并希望线程退出后能够自己释放资源
        //pthread_detach(m_tid_array[i]);//则分离这个线程
        //否则需要记录所创建所有线程的tid, 再析构时对所有线程pthread_join
    }
}
ThreadPool::~ThreadPool(){
    Quit();
    for(size_t i = 0; i < m_max_count; pthread_join(m_tid_array[i], NULL), ++i);
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_pro_cond);
    pthread_cond_destroy(&m_con_cond);
}
void* ThreadPool::thr_start(void* arg){
    ThreadPool* tp = (ThreadPool*)arg;
    while(1){
        pthread_mutex_lock(&(tp->m_mutex));
        while(tp->m_task_q.empty()){
            if(tp->m_exit_flag) {
                ++(tp->m_destroy_count);
                pthread_mutex_unlock(&(tp->m_mutex));
                cout<<"线程退出\n";//用于测试
                pthread_exit(NULL);           
            }
            pthread_cond_wait(&(tp->m_con_cond), &(tp->m_mutex));
        }
        ThreadTask task = tp->m_task_q.front();
        tp->m_task_q.pop();
        pthread_mutex_unlock(&(tp->m_mutex));

        task.Run();//要解锁之后再进行任务处理
        pthread_cond_signal(&(tp->m_pro_cond));
    }
    pthread_exit(NULL);
}
bool ThreadPool::PushTask(const ThreadTask& task){
    pthread_mutex_lock(&m_mutex);
    m_task_q.push(task);
    pthread_mutex_unlock(&m_mutex);

    pthread_cond_signal(&m_con_cond);
    return true;
}
void ThreadPool::Quit(){
    pthread_mutex_lock(&m_mutex);
    m_exit_flag = true;
    pthread_mutex_unlock(&m_mutex);
    while(m_destroy_count != m_max_count) {
        pthread_cond_broadcast(&m_con_cond);
    }
}
#endif
