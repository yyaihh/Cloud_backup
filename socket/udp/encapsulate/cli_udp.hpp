#include<iostream>
#include<string>
#include<functional>
#include"socket_udp.hpp"
using namespace std;

typedef function<void (string* resp)> Handler1;
typedef function<void (const string& req, string* resp)> Handler2;
//req:收到的服务端的请求数据, resp:response客户端要响应服务端的数据(回复的数据) 

class UdpClient{
    UdpSocket m_us;
    string m_ip;
    uint16_t m_port;
public:
    UdpClient(const string ip, const uint16_t port);
    ~UdpClient(){m_us.Close();}
    bool Start(Handler1 handler1, Handler2 handler2);
};
UdpClient::UdpClient(const string ip, const uint16_t port):
    m_ip(ip), m_port(port) {
    m_us.Socket();
}
bool UdpClient::Start(Handler1 handler1, Handler2 handler2){
    string req, resp, ip;
    uint16_t port;
    handler1(&resp);
    while(1){
        if(m_us.Send(resp, m_ip, m_port) == false){
            return false;
        }
        if(m_us.Recv(&req, &ip, &port) == false){
            return false;
        }
        handler2(req, &resp);
        req.clear();
    }
    return true;
}

