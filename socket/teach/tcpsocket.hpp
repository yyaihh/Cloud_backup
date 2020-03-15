/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述：封装一个TcpSocket类;
*             向外提供简单的接口能够实现网络通信编程
================================================================*/
#include <cstdio>//printf
#include <string>//std::string
#include <unistd.h>//close
#include <netinet/in.h>//结构体
#include <arpa/inet.h>//字节序转换接口
#include <sys/socket.h>//套接字接口

#define BACKLOG 10
#define CHECK_RET(q) if((q)==false){return -1;}

class TcpSocket
{
    public:
        TcpSocket():_sockfd(-1){
        }
        //创建套接字
        bool Socket() {
            //socket(地址域， 套接字类型，协议类型)
            _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (_sockfd < 0) {
                perror("socket error");
                return false;
            }
            return true;
        }
        void Addr(struct sockaddr_in *addr, const std::string &ip, uint16_t port) {
            addr->sin_family = AF_INET;
            addr->sin_port = htons(port);
            inet_pton(AF_INET, ip.c_str(), &addr->sin_addr.s_addr);
        }
        //绑定地址信息
        bool Bind(const std::string &ip, uint16_t port) {
            //bind(描述符，地址信息，地址长度)
            struct sockaddr_in addr;
            Addr(&addr, ip, port);
            socklen_t len = sizeof(struct sockaddr_in);
            int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
            if (ret < 0) {
                perror("bind error");
                return false;
            }
            return true;
        }
        //开始监听
        bool Listen(int backlog = BACKLOG) {
            //listen(服务端监听套接字描述符， 同一时间连接数)
            int ret = listen(_sockfd, backlog);
            if (ret < 0) {
                perror("listen error");
                return false;
            }
            return true;
        }
        //客户端发起连接请求
        bool Connect(const std::string &ip, uint16_t port) {
            //1. 定义IPv4地址结构
            struct sockaddr_in addr;
            Addr(&addr, ip, port);
            //2. 向服务端发起连接请求---并且将服务端地址描述到socket中
            // connect(客户端套接字描述符，服务端地址信息， 地址信息长度)
            socklen_t len = sizeof(struct sockaddr_in);
            int ret = connect(_sockfd, (struct sockaddr*)&addr, len);
            if (ret < 0) {
                perror("connect error");
                return false;
            }
            return true;
        }
        //获取新建连接
        bool Accept(TcpSocket *sock, std::string *ip = NULL, uint16_t *port = NULL) {
            //accept(监听套接字描述符，客户端地址信息，地址信息长度)返回操作句柄
            //返回的操作句柄赋值到传入的sock的成员sockfd中，通过sock可以与客户端通信
            struct sockaddr_in cliaddr;
            socklen_t len = sizeof(struct sockaddr_in);
            int newfd = accept(_sockfd, (struct sockaddr*)&cliaddr, &len);
            if (newfd < 0) {
                perror("accept error");
                return false;
            }
            //调用Accept的对象肯定是监听套接字对象
            //传入的TcpSocket对象是新实例化的TcpSocket对象
            sock->_sockfd = newfd;
            if (ip != NULL) {
                *ip = inet_ntoa(cliaddr.sin_addr);//inet_ntoa(struct in_addr);
            }
            if (port != NULL) {
                *port = ntohs(cliaddr.sin_port);
            }
            return true;
        }
        //接收数据
        bool Recv(std::string *buf) {
            //recv(描述符， 缓冲区，想要的数据长度，选项参数-0默认阻塞);
            char tmp[4096] = {0};
            ssize_t ret = recv(_sockfd, tmp, 4096, 0);
            if (ret < 0) {
                perror("recv error");
                return false;
            }else if (ret == 0) {//连接已经断开
                printf("connection break\n");
                return false;
            }
            buf->assign(tmp, ret);//从tmp中拷贝ret大小的数据到buf中
            return true;
        }
        //发送数据
        bool Send(const std::string &data) {
            //send(描述符，数据，数据长度，选项参数)
            ssize_t ret = send(_sockfd, data.c_str(), data.size(), 0);
            if (ret < 0) {
                perror("send error");
                return false;
            }
            //不太严谨---send有可能想要发送100个字节，但是实际只发送10个字节
            //while(slen < data.size)
            // slen += send(sockfd, &data[slen], data.size()-slen, 0);
            return true;
        }
        //关闭套接字
        bool Close() {
            close(_sockfd);
            _sockfd = -1;
            return true;
        }
    private:
        int _sockfd;
};

