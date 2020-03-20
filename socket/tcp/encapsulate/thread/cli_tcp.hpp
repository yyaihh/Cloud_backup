#include<iostream>
#include<functional>
#include"socket_tcp.hpp"
using namespace std;

typedef function<void (string* resp)> Handler1;
typedef function<void (const string& req, string* resp)> Handler2;
//req:收到的服务端的请求数据, resp:response客户端要响应服务端的数据(回复的数据)

class TcpClient {
    TcpSocket m_ts;
    string m_ip;
    uint16_t m_port;
public:
    TcpClient(const string ip, const uint16_t port);
    ~TcpClient(){m_ts.Close();}
    bool Strat(Handler1 handler1, Handler2 handler2);
};

TcpClient::TcpClient(const string ip, const uint16_t port):
    m_ip(ip), m_port(port) {
    m_ts.Socket();
}
bool TcpClient::Strat(Handler1 handler1, Handler2 handler2){
    if(m_ts.Connect(m_ip, m_port) == false){
        return false;
    }
    string req, resp;
    handler1(&resp);
    while(1){
        if(m_ts.Send(resp) == false){
            return false;
        }
        if(m_ts.Recv(&req) == false){
            return false;
        }
        handler2(req, &resp);
        req.clear();
    }
    return true;
}
