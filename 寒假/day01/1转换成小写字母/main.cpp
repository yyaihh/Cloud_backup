#include<iostream>
#include<string>
using namespace std;
class Solution {
    public:
        string toLowerCase(string str) {
            for(auto & s : str){
                if(s>='A'&&s<='Z'){
                    s+=('a'-'A');
                }
            }
            return str;
        }

};
int main(){
    string str;
    Solution s;
    while(cin>>str){
        cout<<s.toLowerCase(str)<< endl;
    }
    return 0;
}
