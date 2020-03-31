#include<iostream>
#include"cloud_backup.hpp"
using namespace std;

void not_compress(){
    cloud_sys::NotHot nh;
    if(boost::filesystem::exists(GZBACKUP_PATH) == false){
        boost::filesystem::create_directory(GZBACKUP_PATH);

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
