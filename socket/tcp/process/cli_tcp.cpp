#include<iostream>
#include<cstdio>
#include<cstdlib>
#include"socket_tcp.hpp"

/*
1. 创建套接字
2. 绑定地址信息（不推荐）
3. 向服务端发起连接请求
    while(1) {
        4. 先发送数据
        5. 接收数据
    } 
6. 关闭套接字*/
int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "Should input ./cli_tcp [ip] [port]\n";
        return -1;
    }
    TcpSocket cli;
    CHECK_RET(cli.Socket());
    string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    CHECK_RET(cli.Connect(ip, port));
    //开始持续通信
    string buf;
    while(1){
        cout << "client say : ";
        getline(cin, buf);
        CHECK_RET(cli.Send(buf));
        buf.clear();
        CHECK_RET(cli.Recv(&buf));
        cout << "servers say : " << buf << endl;
    }
    return 0;
}
