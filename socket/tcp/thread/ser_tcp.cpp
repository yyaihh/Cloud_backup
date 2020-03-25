#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<pthread.h>
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

void* thr_start(void* arg) {
    pthread_detach(pthread_self());
    TcpSocket* sock = (TcpSocket*)arg;
    string buf;
    while(1){
        if (sock->Recv(&buf) == false) {
            delete sock;
            break;
        }
        cout << "client say: " << buf << endl;
        cout << "serves say: "; 
        getline(cin, buf);
        if(sock->Send(buf) == false){
            delete sock;
            break;
        }
    }
    return NULL;
}
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
        TcpSocket* newsock = new TcpSocket;
        string ip;
        uint16_t port;
        bool ret = ser.Accept(newsock, &ip, &port);
        if(ret == false){delete newsock;  continue; }//服务端并不会因为一次失败而退出, 而是继续获取下一个连接
        printf("new connection[ip: %s][port: %d]\n", ip.c_str(), port);
        pthread_t tid;
        if(pthread_create(&tid, NULL, thr_start, (void*)newsock)){
            fprintf(stderr, "pthread_create: %s\n", strerror(ret));
            delete newsock;
            return -1;
        }
    }
    return 0;
}
