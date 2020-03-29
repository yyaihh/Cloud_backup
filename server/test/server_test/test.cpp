#include<iostream>
#include"cloud_backup.hpp"
using namespace std;

void not_compress(){
    cloud_sys::NotHot nh;
    if(boost::filesystem::exists(nh.m_s_ComFilePath) == false){
        boost::filesystem::create_directory(nh.m_s_ComFilePath);

    }
    nh.Start();
    return;
}

void server_start(){
    cloud_sys:: Server server;
    server.Start();
}

int main(){
    string name("file");
    cloud_sys::data_manage.Insert(name, name);
    thread thr(not_compress);
    thread thr1(server_start);
    thr.join();
    thr1.join();
    return 0;
}
