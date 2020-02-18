#include<iostream>
using namespace std;

int main(){
    const int a = 10;
    int* p = const_cast<int*>(&a);
    *p = 0;
    cout << a << endl;
    cout << *p << endl;
    volatile const int b = 10;
    int* p2 = const_cast<int*>(&b);
    *p2 = 0;
    cout << b << endl;  
    cout << *p << endl; 
    return 0;
}
