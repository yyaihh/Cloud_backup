#include<stdio.h>
void func(){ 
    int i = 0;
    for(;i < 10; ++i){
        if(i%2==0){
            printf("%d\n", i);
        }
    }
}
int main(){
    func();
    return 0;
}
