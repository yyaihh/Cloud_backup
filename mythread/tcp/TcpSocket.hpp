#include<iostream>

class TcpSocket{
public:
    TcpSocket(){

    }
    //创建套接字
    bool Socket(){
        m_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
};
