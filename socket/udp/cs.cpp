#include<iostream>
#include<cstdio>
#include<cstdlib>
#include"udp_client.h"
using namespace std;
#define CHECK_RET(ret) if((ret) == false){return -1;}
int main(int argc, char* argv[]){
    //传入的是服务器端的ip和端口
    if(argc != 3){

    }
    string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    UdpSocket client;
    CHECK_RET(client.Socket());
    //CHECK_RET(client.bind());
    while(1){
        cout << "请输入: ";
        string buf;
        cin >> buf;
        client.Send(buf,ip, port);
        buf.clear();
        client.Recv(&buf);
        cout << "男朋友说:" << buf << endl;
    }
    client.Close();
    return 0;
}
