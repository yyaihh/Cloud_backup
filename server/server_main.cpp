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
    thread thr1(server_start);
    thread thr(not_compress);
    thr.join();
    thr1.join();
    return 0;
}
