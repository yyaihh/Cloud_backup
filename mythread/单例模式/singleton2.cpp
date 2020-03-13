#include<iostream>
#include<mutex>
#include<cstdio>
using namespace std;
class singleton{
    volatile static int* m_data;
    mutex m_mutex;
public:
    volatile int* get_instance(){
        if(m_data == nullptr){
            m_mutex.lock();
            if(m_data == nullptr){
                m_data = new int;
            }   
            m_mutex.unlock();
        }
        return m_data;
    }
};
volatile int* singleton::m_data = nullptr;
int main(){
    singleton a;
    singleton b;
    printf("%p\n", a.get_instance());
    printf("%p\n", b.get_instance());
    return 0;
}
