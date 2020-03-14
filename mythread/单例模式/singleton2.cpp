#include<iostream>
#include<mutex>
#include<cstdio>
using namespace std;
template<class T>
class singleton{
    volatile static T* m_data;
    mutex m_mutex;
public:
    volatile T* get_instance(){
        if(m_data == nullptr){
            m_mutex.lock();
            if(m_data == nullptr){
                m_data = new int;
            }   
            m_mutex.unlock();
        }
        return m_data;
    }
    ~singleton(){
        if(m_data){
            delete m_data;
            m_data = nullptr;
        }
    }
};
template<class T>
volatile T* singleton<T>::m_data = nullptr;
int main(){
    singleton<int> a;
    singleton<int> b;
    printf("%p\n", a.get_instance());
    printf("%p\n", b.get_instance());
    return 0;
}
