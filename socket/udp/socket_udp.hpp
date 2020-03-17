#include<iostream>
#include<cstdio>
#include<string>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
using namespace std;

class UdpSocket{
    int m_sockfd;
public:
    bool Socket(){
        m_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if(m_sockfd < 0){
            perror("socket error");
            return false;
        }
        return true;
    }
    void Addr(struct sockaddr_in* addr, const string& ip, const uint16_t port){
        addr->sin_family = AF_INET;
        addr->sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &addr->sin_addr.s_addr);
    }
    bool Bind(const string& ip, const uint16_t port){
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

    bool Recv(string* buf, string* dst_ip = NULL, uint16_t* dst_port = NULL){
        char tmp[4096] ={ 0 };
        struct sockaddr_in addr;
        socklen_t len = sizeof(struct sockaddr_in);
        int ret = recvfrom(m_sockfd, tmp, 4093, 0, (struct sockaddr*)&addr, &len);
        //len在此处是输入输出型, 即指定了想获得多少, 并且还返回了实际获得了多少
        if(ret < 0){
            perror("recvfrom error");
            return false;
        }
        buf->assign(tmp, ret);
        if(dst_ip != NULL){
            *dst_ip = inet_ntoa(addr.sin_addr);
        }
        if(dst_port != NULL){
            *dst_port = ntohs(addr.sin_port);
        }
        return true;
    }
    bool Send(const string& data, const string& ip, const uint16_t port){
        struct sockaddr_in addr;
        Addr(&addr, ip, port);
        socklen_t len = sizeof(sockaddr_in);
        int ret = sendto(m_sockfd, data.c_str(), data.size(), 0, (struct sockaddr*)&addr, len);
        if(ret < 0){
            perror("sendto error");
            return false;
        }
        return true;
    }
    bool Close(){
        close(m_sockfd);
        return true;
    }
};
