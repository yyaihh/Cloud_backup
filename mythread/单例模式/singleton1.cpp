#include<iostream>
using namespace std;
class singleton{
    static int m_data;
public:
    int* gey_instance(){
        return &m_data;
    }
};
int singleton::m_data = 0;
int main(){
    singleton a;
    singleton b;
    cout << a.gey_instance() << endl;
    cout << b.gey_instance() << endl;
    return 0;
}
