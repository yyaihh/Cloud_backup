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
        printf("输入错误\n");
        return -1;
    }
    TcpSocket ser;
    ser.Socket();
    string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    ser.Bind(ip, port);
    ser.Listen();
    string buf;
    while(1){
        TcpSocket newsock;
        string ip;
        uint16_t port;
        bool ret = ser.Accept(&newsock, &ip, &port);
        cout << "建立新链接,ip:" << ip << "端口号:" << port << endl;
        ret = newsock.Recv(&buf);
        if (ret == false) {newsock.Close(); continue;}
        cout << "client say: " << buf << endl;
        cout << "serves say: "; 
        getline(cin, buf);
        ret = newsock.Send(buf);
        if (ret == false) {newsock.Close(); continue;}
    }
    ser.Close();
    return 0;
}
