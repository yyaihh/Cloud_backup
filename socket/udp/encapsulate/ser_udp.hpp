#include<iostream>
#include<functional>
#include"socket_udp.hpp"
using namespace std;

typedef function<void (const string& req, string* resp)> Handler;
//req:收到的服务端的请求数据, resp:response客户端要响应服务端的数据(回复的数据) 
class UdpServer{
    UdpSocket m_us;
    string m_ip;
    uint16_t m_port;
public:
    UdpServer(const string ip, const uint16_t port);
    ~UdpServer(){m_us.Close();}
    bool Start(Handler handler);
};

UdpServer::UdpServer(const string ip, const uint16_t port):
    m_ip(ip), m_port(port) {
    m_us.Socket();
}
bool UdpServer::Start(Handler handler){
    string ip;
    uint16_t port;
    m_us.Bind(m_ip, m_port);
    while(1){
        string resp, req;
        if(m_us.Recv(&req, &ip, &port) == false){
            return false;
        }
        handler(req, &resp);
        if(m_us.Send(resp, ip, port) == false){
            return false;
        }
    }
    return true;
}
