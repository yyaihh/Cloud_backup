#include<iostream>
#include<string>
#include<unistd.h> //close包含在这个头文件中
#include<netinet/in.h>//地址结构体
#include<arpa/inet.h>//字节序转换接口
#include<sys/socket.h>//套接字接口

using namespace std;
#define BACKLOG 10
#define CHECK_RET(ret){if((ret) == false) return -1;} 

class TcpSocket{
    int m_sockfd;

    void Addr(struct sockaddr_in*, const string&, const uint16_t);
public:
    TcpSocket():m_sockfd(-1){}
    ~TcpSocket(){Close();}
    bool Socket();
    bool Bind(const string& ip, const uint16_t port);
    bool Listen(int backlog = BACKLOG);
    bool Connect(const string& ip, const uint16_t port);
    bool Accept(TcpSocket* sock, string* ip = nullptr, uint16_t* port = nullptr);
    bool Recv(string* buf);
    bool Send(const string& data);
    bool Close();
};

bool TcpSocket::Socket(){
    m_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_sockfd < 0){
        perror("socket error");
        return false;
    }
    return true;
}
void TcpSocket::Addr(struct sockaddr_in* addr, const string& ip, const uint16_t port){
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr->sin_addr.s_addr);
}
bool TcpSocket::Bind(const string& ip, const uint16_t port){
    struct sockaddr_in addr;
    Addr(&addr, ip, port);
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = bind(m_sockfd, (struct sockaddr*)&addr, len);
    if(ret < 0){
        perror("bind error");
        return false;
    }
    return true;
}

bool TcpSocket::Listen(int backog){
    int ret = listen(m_sockfd, backog);
    if(ret < 0){
        perror("listen error");
        return false;
    }
    return true;
}

bool TcpSocket::Connect(const string& ip, const uint16_t port){
    struct sockaddr_in addr;//定义一个IPv4的地址结构
    Addr(&addr, ip, port);//向这个结构中绑定地址与端口
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = connect(m_sockfd, (struct sockaddr*)&addr, len);
    //将服务端信息描述到socket中, 并向服务端发起连接请求
    if(ret < 0){
        perror("connect error");
        return false;
    }
    return true;
}

bool TcpSocket::Accept(TcpSocket* sock, string* ip, uint16_t* port){
    struct sockaddr_in cli_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int newsockfd = accept(m_sockfd, (struct sockaddr*)&cli_addr, &len);
    if(newsockfd < 0){
        perror("accept error");
        return false;
    }
    sock->m_sockfd = newsockfd;
    if(ip != nullptr){
        *ip = inet_ntoa(cli_addr.sin_addr);
    }
    if(port != nullptr){
        *port = ntohs(cli_addr.sin_port);
    }
    return true;
}

bool TcpSocket::Recv(string* buf){
    char tmp[4096] = { 0 };
    ssize_t ret = recv(m_sockfd, tmp, 4095, 0);
    if(ret < 0){
        perror("recv error");
        return false;
    }
    else if(ret == 0){
        cout << "connection break\n";
        return false;
    }
    buf->assign(tmp, ret);
    return true;
}

bool TcpSocket::Send(const string& data){
    /*ssize_t ret = send(m_sockfd, data.c_str(), data.size(), 0);
    if(ret < 0){
        perror("send error");
        return false;
    }*/
    //send有这个问题, 比如send有想发送100字节的数据, 但实际可
    //能一次发送的数据并没有100字节, 所以需要循环发送, 如下
    size_t slen = 0, ret = 0;
    size_t size = data.size();
    while(slen < size) {
        ret = send(m_sockfd, &data[slen], size - slen, 0);
        if(ret < 0){
            perror("send error");
            return false;
        }
        slen += ret;
    }
    return true;
}
bool TcpSocket::Close(){
    close(m_sockfd);
    m_sockfd = -1;
    return true;
}
