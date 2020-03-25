#ifndef __SOMEFILE_H__
#define __SOMEFILE_H__
#include<iostream>
#include<vector>
#include<pthread.h>
#include<semaphore.h>
using namespace std;
#endif

#define MAX_QUEUE 5

template<class T>
class RingQueue{
    vector<T> m_array;
    size_t m_max_capacity;
    int m_pos_real;
    int m_pos_front;
    sem_t m_sem_data;//数据资源计数器
    sem_t m_sem_free;//空闲空间计数器
    sem_t m_sem_lock;//实现互斥锁
public:
    RingQueue(size_t max_capacity = MAX_QUEUE):
        m_array(max_capacity),
        m_max_capacity(max_capacity),
        m_pos_real(0), 
        m_pos_front(0) {
        sem_init(&m_sem_data, 0, 0);
        sem_init(&m_sem_free, 0, max_capacity);
        sem_init(&m_sem_lock, 0, 1);
    }
    ~RingQueue(){
        sem_destroy(&m_sem_data);
        sem_destroy(&m_sem_free);
        sem_destroy(&m_sem_lock);
    }
    void Push(const T& data);
    void Pop(T* data);
};
template<class T>
void RingQueue<T>::Push(const T& data){
    sem_wait(&m_sem_free);//通过自身计数判断, 是否有剩余空间, 若为0则阻塞,
    //若>0则计数 - 1
    sem_wait(&m_sem_lock);//先加锁, 保护入队操作, 计数为1则不阻塞, 计数-1, 否则阻塞
    m_array[m_pos_real] = data;
    m_pos_real = (m_pos_real + 1) % m_max_capacity;
    cout << "生产线程:0x" << pthread_self() << hex  <<": 我生产了一份数据\n";//为了测试直观
    sem_post(&m_sem_lock);//解锁, 计数+1, 并唤醒等待锁的生产者
    sem_post(&m_sem_data);//数据资源计数+1, 并唤醒消费者
}
template<class T>
void RingQueue<T>::Pop(T* data){
    sem_wait(&m_sem_data);

    sem_wait(&m_sem_lock);
    *data = m_array[m_pos_front];
    m_pos_front = (m_pos_front + 1) % m_max_capacity;
    cout << "消费者线程:0x" << pthread_self() << hex  <<": 我拿到了一份数据\n";//为了测试直>    观     
    sem_post(&m_sem_lock);

    sem_post(&m_sem_free);
}
