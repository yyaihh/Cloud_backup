#include<iostream>
#include<cstdio>
#include<cstdlib>
#include"socket_tcp.hpp"

/*1. 创建套接字
2. 绑定地址信息
3. 开始监听
while(1) {
    4. 获取新连接---一个服务器不可能只跟一个客户端通信
    5. 通过新连接接收数据
    6. 通过新连接发送数据
}
7. 关闭套接字*/


int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "Should input ./ser_tcp [ip] [port]\n";
        return -1;
    }
    TcpSocket ser;
    CHECK_RET(ser.Socket());
    string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    CHECK_RET(ser.Bind(ip, port));
    CHECK_RET(ser.Listen());
    string buf;
    while(1){
        TcpSocket newsock;
        string ip;
        uint16_t port;
        bool ret = ser.Accept(&newsock, &ip, &port);
        if(ret == false){ continue; }//服务端并不会因为一次失败而退出, 而是继续获取下一个连接
        printf("new connection[ip: %s][port: %d]\n", ip.c_str(), port);  
        ret = newsock.Recv(&buf);
        if (ret == false) {
            break;
        }
        cout << "client say: " << buf << endl;
        cout << "serves say: "; 
        getline(cin, buf);
        ret = newsock.Send(buf);
    }
    return 0;
}
