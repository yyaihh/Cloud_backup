#include<iostream>
using namespace std;
template<class T>
class singleton{
    static T m_data;
public:
    T* gey_instance(){
        return &m_data;
    }
};
template<class T>
T singleton<T>::m_data = 0;

int main(){
    singleton<int> a;
    singleton<int> b;
    cout << a.gey_instance() << endl;
    cout << b.gey_instance() << endl;
    return 0;
}
