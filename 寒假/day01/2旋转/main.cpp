#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
class Solution {
public:
    void rotate1(vector<int>& nums, int k) {//n^2的复杂度, 太慢
        size_t n = nums.size();
        int tmp;
        while(k--){
            tmp = nums[n - 1];
            for(size_t i = n - 1;i > 0; --i){
                nums[i] = nums[i - 1];
            }                
            nums[0] = tmp;
        }
    }
    void rotatel2(vector<int>& nums, int k){
        int l = nums.size();
        k = k % l;
        reverse(nums.begin(), nums.begin() + l - k);
        reverse(nums.begin() + l - k, nums.end());
        reverse(nums.begin(), nums.end());
    }

};

int main(){
    Solution s;
    vector<int> v = {1,2,3,4,5,6,7};
    int k;
    cin >> k;
    s.rotatel2(v, k);
    for(auto& e : v){
        cout << e << " ";
    }
    return 0;
}
