#include<iostream>
#include"cloud_backup.hpp"

void m_non_compress(){
    cloud_sys::NotHot nh;
    if(boost::filesystem::exists(nh.m_s_ComFilePath) == false){
        boost::filesystem::create_directory(nh.m_s_ComFilePath);
    }
    nh.Start();
    return;
}
int main(int argc, char* argv[]){
    //非热点文件压缩路径不存在则创建新的
    //文件备份路径不存在则创建新的
    string name("file");
    cloud_sys::data_manage.Insert(name, name);
    thread thr(m_non_compress);
    thr.join();
    return 0;
}
