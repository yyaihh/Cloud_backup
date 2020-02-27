#include<iostream>
#include<queue>
using namespace std;
class BlockQueue {
        std::queue<int> m_q;//安全队列
        size_t m_capacity; //最大容量
        pthread_mutex_t  m_mutex;//互斥锁
        pthread_cond_t  m_cond_pro;//生产者
        pthread_cond_t  m_cond_con;//消费者
public:
        BlockQueue(){

        }
        ~BlockQueue(){

        }
        bool Push(int& data){

        }
        bool Pop(int* data){

        }
};
