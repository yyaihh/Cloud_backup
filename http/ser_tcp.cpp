#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<pthread.h>
#include<sstream>
#include"socket_tcp.hpp"


void* thr_start(void* arg) {
    pthread_detach(pthread_self());
    TcpSocket* sock = (TcpSocket*)arg;
    string buf;
    if (sock->Recv(&buf) == false) {
        delete sock;
        return NULL;
    }
    cout << "http req:\n" << buf;
    string body = "<html><body><h1>Hello World!</h1></body></html>";
    string blank = "\r\n";
    stringstream header;
    header << "Content-Length: " << body.size() << "\r\n";
    header << "Content-Type: text/html\r\n";
    //header << "Location: http://www.baidu.com/\r\n";
    //string first = "HTTP/1.1 302 Found\r\n";
    string first = "HTTP/1.1 200 OK\r\n";
    if(sock->Send(first) == false){
        delete sock;
        return NULL;
    }
    if(sock->Send(header.str())){
        delete sock;
        return NULL;
    }
    if(sock->Send(blank)){
        delete sock;
        return NULL;
    }
    if(sock->Send(body)){
        delete sock;
        return NULL;
    }
    sleep(5);
    delete sock;
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
        if(ret == false){ delete newsock; continue; }//服务端并不会因为一次失败而退出, 而是继续获取下一个连接
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
