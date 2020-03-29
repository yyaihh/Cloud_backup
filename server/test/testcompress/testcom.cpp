#include<iostream>
#include"cloud_backup.hpp"

int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "Should input ./testcom 源文件名称 压缩包名称\n";
        return -1;
    }
    string src = argv[1];
    string dst = argv[2];
    cout << "开始压缩\n";
    cloud_sys::CompressUtil::Compress(src, dst);
    cout << "压缩结束\n";
    string filename = argv[1];
    filename += ".txt";
    cout << "开始解压\n";
    cloud_sys::CompressUtil::UnCompress(src, filename);
    cout << "解压结束\n";
    return 0;
}
