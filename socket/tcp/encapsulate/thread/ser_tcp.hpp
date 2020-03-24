#include<iostream>
#include<cstring>
#include<functional>
#include<pthread.h>
#include"socket_tcp.hpp"

typedef function<void (const string&, string*, const string&, const uint16_t)> Handler;

class Data{
public:
    TcpSocket m_newsock;
    string m_ip;
    uint16_t m_port;
    Handler m_handler;
};

class TcpThreadServer{
    TcpSocket m_listen_sock;
    string m_ip;
    uint16_t m_port;

    static void* ThreadConnect(void* arg);
public:
    TcpThreadServer(const string ip, const uint16_t port):
        m_ip(ip), m_port(port) {
        m_listen_sock.Socket();
    }
    ~TcpThreadServer(){m_listen_sock.Close();}
    bool Start(Handler handler, int backlog = BACKLOG);
};
void* TcpThreadServer::ThreadConnect(void* arg){
    pthread_detach(pthread_self());
    Data* data = (Data*)arg;
    string req, resp;
    while(1){
        if(data->m_newsock.Recv(&req) == false) {
            delete data;
            break;
        }
        data->m_handler(req, &resp, data->m_ip, data->m_port);
        if(data->m_newsock.Send(resp) == false){
            delete data;
            break;
        }
    }
    return NULL;
}
bool TcpThreadServer::Start(Handler handler, int backlog){
    CHECK_RET(m_listen_sock.Bind(m_ip, m_port));
    CHECK_RET(m_listen_sock.Listen(backlog));
    while(1){
        Data* data = new Data;
        data->m_handler = handler;
        if(m_listen_sock.Accept(&data->m_newsock, &data->m_ip, &data->m_port) == false){
            delete data;
            continue;
        }
        printf("new connection[ip: %s][port: %d]\n", data->m_ip.c_str(), data->m_port);
        pthread_t tid;
        int ret = pthread_create(&tid, NULL, ThreadConnect, (void*)(data));
        if(ret){
            fprintf(stderr, "pthread_create:%s\n", strerror(ret));
            delete data;
            return false;
        }
    }
    return true;
}

