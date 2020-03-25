#include<iostream>
#include<string>
using namespace std;
class Solution {
public:
    bool isPalindrome(int x) {
        if(x<0){
            return false;                            
        }
        int tmp = x;
        long res = 0;
        while(x){
            res*=10;
            res+=(x%10);
            x/=10;
        }
        return res == tmp;                                    
    }
    bool canConstruct(string& ransomNote, string& magazine) {
        int a[26]={0};
        for(auto& c:magazine){ 
            ++a[c-97];
        }
        for(auto& c:ransomNote){ 
            --a[c-97];
        }
        for(int i = 0;i<26;++i){
            if(a[i]<0){
                return false;
            }
        }
        return true;
    }
};

int main(){

    return 0;
}
