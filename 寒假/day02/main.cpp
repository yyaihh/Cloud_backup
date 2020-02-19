#include<iostream>
#include<vector>
using namespace std;
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        auto i = nums.begin();
        auto j = i;
        for(;i != nums.end(); ++i){
            if(*i != val){
                *j = *i;
                ++j;
            }
        }
        return j - nums.begin();
    }

    int removeElement1(vector<int>& nums, int val) {
        auto l = nums.end();
        --l;
        for(auto i = nums.begin(); i != nums.end();){
            if(*i == val) {
                *i = *l;
                nums.pop_back();
                --l;
                continue;
            }
            ++i;
        }
        return nums.size(); 
    }
};
int main(){
    vector<int> v = {1, 2, 2, 4, 6 ,7, 4, 2, 9, 10};
    int val = 2;
    Solution s;
    for(auto& e : v){
        cout << e << "  ";
    }
    cout << endl;
    s.removeElement(v, val);
    for(auto& e : v){
        cout << e << "  ";
    }
    cout << endl;
    return 0;
}
