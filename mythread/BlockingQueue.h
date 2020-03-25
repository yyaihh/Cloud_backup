#ifndef __SOMEFILE_H__
#define __SOMEFILE_H__
#include<iostream>
#include<queue>
#include<pthread.h>
using namespace std;
#endif

#define MAX_QUEUE 5

template<class T>
class BlockingQueue{
    queue<T> m_q;//安全队列
    size_t m_max_capacity;//最大容量
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond_pro;//生产者
    pthread_cond_t m_cond_con;//生产者
public:
    BlockingQueue(size_t capacity = MAX_QUEUE) 
        : m_max_capacity(capacity)
    {
        pthread_mutex_init(&m_mutex, NULL);
        pthread_cond_init(&m_cond_con, NULL);
        pthread_cond_init(&m_cond_pro, NULL);
    }
    ~BlockingQueue(){
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_cond_con);
        pthread_cond_destroy(&m_cond_pro);
    }
    void Push(const T& data);
    void Pop(T* data);
};
template<class T>
void BlockingQueue<T>::Push(const T& data){
    pthread_mutex_lock(&m_mutex);
    while(m_q.size() == m_max_capacity){
        pthread_cond_wait(&m_cond_pro, &m_mutex);
    }
    m_q.push(data);
    cout << "生产线程:0x" << pthread_self() << hex  <<": 我生产了一份数据\n";//为了测试直观
    pthread_mutex_unlock(&m_mutex);
    pthread_cond_signal(&m_cond_con);
}
template<class T>
void BlockingQueue<T>::Pop(T* data){
    pthread_mutex_lock(&m_mutex);
    while(m_q.empty()){
        pthread_cond_wait(&m_cond_con, &m_mutex);
    }
    *data = m_q.front();
    m_q.pop();
    cout << "消费者线程:0x" << pthread_self() << hex  <<": 我拿到了一份数据\n";//为了测试直观
    pthread_mutex_unlock(&m_mutex);
    pthread_cond_signal(&m_cond_pro);
}

