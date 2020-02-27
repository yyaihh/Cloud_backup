#include<iostream>
#include<queue>
using namespace std;
class RingQueue{
       vector<int> m_array;//数组
       int m_capacity;//数组元素个数
       int m_pos_write;//写指针
       int m_pos_read;//读指针
       sem_t  m_sem_pro; //空闲空间计数器, 有多少个空闲空间, 就可以生产所少个数据
       sem_t  m_set_con; //数据资源计数器, 有多少个数据资源, 就可以获取到多少个数据
       sem_t  m_sem_lock;//实现安全的锁
};
