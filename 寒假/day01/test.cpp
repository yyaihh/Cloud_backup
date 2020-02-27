#include<iostream>
#include<typeinfo>
using namespace std;
int main(){
    int a;
    a = 'A' + 1.6;
    cout << a << endl;
    cout << typeid(a).name() << endl
        << typeid(int).name() << endl
        << typeid(unsigned int).name() << endl;
    return 0;
}
