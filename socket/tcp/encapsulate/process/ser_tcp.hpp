//多进程
#include<iostream>
#include<functional>
#include<sys/wait.h>
#include"socket_tcp.hpp"

typedef std::function<void (const string&, string*, const string&, const uint16_t&)> Handler;

void sigcb(int signo){
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

class TcpProcessServer{
    TcpSocket m_listen_scok;
    string m_ip;
    uint16_t m_port;

    void ProcessConnect(const TcpSocket& newsock, const string& ip, 
                    const uint16_t port, Handler handler);
public:
    TcpProcessServer(const string ip, const uint16_t port);
    ~TcpProcessServer(){m_listen_scok.Close();}
    bool Start(Handler handler, int backlog = BACKLOG);
};

TcpProcessServer::TcpProcessServer(const string ip, const uint16_t port):
    m_ip(ip), m_port(port) {
    signal(SIGCHLD, sigcb);
    m_listen_scok.Socket();
}

void TcpProcessServer::ProcessConnect(const TcpSocket& newsock, const string& ip,
        const uint16_t port, Handler handler){
    pid_t pid = fork();
    if(pid > 0){
        return;
    }
    else if(pid == 0){
        while(1){
            string req, resp;
            if(newsock.Recv(&req) == false){
                return;
            }
            handler(req, &resp, ip, port);
            if(newsock.Send(resp) == false){
                return;
            }
        }
    }
    else{
        perror("fork error");
        return;
    }
}

bool TcpProcessServer::Start(Handler handler, int backlog){
    CHECK_RET(m_listen_scok.Bind(m_ip, m_port));
    CHECK_RET(m_listen_scok.Listen(backlog));
    while(1){
        TcpSocket newsock;
        string ip;
        uint16_t port;
        if(m_listen_scok.Accept(&newsock, &ip, &port) == false){
            continue;
        }
        printf("new connection[ip: %s][port: %d]\n", ip.c_str(), port);
        ProcessConnect(newsock, ip, port, handler);
    }
    return true;
}
