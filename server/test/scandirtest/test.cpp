#include<iostream>
#include<string>
#include<boost/filesystem.hpp>
using namespace std;
int main(){
    string dir = "./";
    boost::filesystem::directory_iterator begin(dir);//定义目录迭代器
    boost::filesystem::directory_iterator end;
    //boost::filesystem::is_directory()//判断文是否是目录
    for(auto it = begin; it != end; ++it){
        //it->status()文件的属性
        if(boost::filesystem::is_directory(it->status())){
            continue;
        }
        cout << it->path().string() << endl;//带路径的
        //path并不是string, 需要提供的接口string()返回一个string
        cout << it->path().filename().string() << endl;//不带路径的
    }
    return 0;
}

