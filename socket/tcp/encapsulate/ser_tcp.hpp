#include<iostream>
#include<string>
#include<functional>
#include"socket_tcp.hpp"

typedef  std::function<void (const string& req, string* resp)> Handler;
//获取到到数据后的处理函数
//req:收到的服务端的请求数据, resp:response客户端要响应服务端的数据(回复的数据) 

class TcpServer{
    TcpSocket m_listen_sock;
    string m_ip;
    uint16_t m_port;
public:
    TcpServer(const string ip, const uint16_t port);
    ~TcpServer(){m_listen_sock.Close();}
    bool Start(Handler handler);
};

TcpServer::TcpServer(const string ip, const uint16_t port): 
    m_ip(ip), m_port(port) {
    m_listen_sock.Socket();
}
bool TcpServer::Start(Handler handler){
    //1.绑定ip与端口号
    CHECK_RET(m_listen_sock.Bind(m_ip, m_port));
    //2.进行监听
    CHECK_RET(m_listen_sock.Listen(10));
    //3.进行事件循环
    while(1){
        TcpSocket newsock;
        string ip;
        uint16_t port;
        //4.获取新连接
        bool ret = m_listen_sock.Accept(&newsock, &ip, &port);
        if(ret == false){
            continue;
        }
        printf("new connection[ip: %s][port: %d]\n", ip.c_str(), port);
        //5.循环收发
        while(1){
            string req;
            //6.读取请求, 读取失败则结束循环
            ret = newsock.Recv(&req);
            if(ret == false){//需要关闭newsock文件描述符, 在析构中已完成
                break;
            }
            //7.数据处理
            string resp;
            handler(req, &resp);
            //8.回复请求
            newsock.Send(resp);
        }
    }
    return true;
}

