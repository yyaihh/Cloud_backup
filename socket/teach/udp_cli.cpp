/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述：这个demo演示封装一个udpsocket类 
================================================================*/

#include <cstdio>
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class UdpSocket
{
    public:
        //创建套接字
        bool Socket() {
            // 地址域，套接字类型，协议类型
            _sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (_sockfd < 0) {
                perror("socket error");
                return false;
            }
            return true;
        }
        void Addr(struct sockaddr_in *addr, const std::string &ip, const uint16_t port) {
            addr->sin_family = AF_INET;
            addr->sin_port = htons(port);
            inet_pton(AF_INET, ip.c_str(), &addr->sin_addr.s_addr);
        }
        //绑定地址信息
        bool Bind(const std::string &ip, const uint16_t port) {
            struct sockaddr_in addr;
            Addr(&addr, ip, port);
            socklen_t len = sizeof(struct sockaddr_in);
            //描述符， 地址信息， 地址长度
            int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
            if (ret < 0) {
                perror("bind error");
                return false;
            }
            return true;
        }
        //接收数据
        bool Recv(std::string *buf, std::string *dst_ip = NULL, uint16_t *dst_port = NULL) {
            char tmp[4096] = {0};
            struct sockaddr_in addr;
            socklen_t len = sizeof(struct sockaddr_in);
            //描述符，缓冲区，长度，选项，对端地址缓冲区，地址长度缓冲区
            int ret = recvfrom(_sockfd, tmp, 4096, 0, (struct sockaddr*)&addr, &len);
            if (ret < 0) {
                perror("recvfrom error");
                return false;
            }
            buf->assign(tmp, ret); // 给buf开辟ret长度的空间，然后从tmp中拷贝ret长度的数据过来
            if (dst_ip != NULL) {//若用户传入了一个空间的地址，则为空间赋值IP地址信息
                //inet_ntoa(struct in_addr*) - 将网络字节序转换为字符串IP地址，并返回字符串首地址
                *dst_ip = inet_ntoa(addr.sin_addr);
            }
            if (dst_port != NULL) {
                *dst_port = ntohs(addr.sin_port);
            }
            return true;
        }
        //发送数据
        bool Send(const std::string &data, const std::string &ip, const uint16_t &port) {
            struct sockaddr_in addr;
            socklen_t len = sizeof(struct sockaddr_in);
            Addr(&addr, ip, port);
            //sendto(描述符，数据，长度，选项， 对端地址，地址长度)
            int ret = sendto(_sockfd, data.c_str(), data.size(), 0, (struct sockaddr*)&addr, len);
            if (ret < 0) {
                perror("sendto error");
                return false;
            }
            return true;
        }
        //关闭套接字
        bool Close() {
            close(_sockfd);
            return true;
        }
    private:
        int _sockfd;
};

#define CHECK_RET(q) if((q)==false){return -1;}
int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("em: ./udp_cli 192.168.122.132 9000 --- 这是服务端的地址\n");
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    UdpSocket client;
    CHECK_RET(client.Socket());
    //CHECK_RET(client.Bind());
    while(1) {
        printf("client say: ");
        fflush(stdout);
        std::string buf;
        std::cin >> buf;
        client.Send(buf, ip, port);

        buf.clear();
        client.Recv(&buf);
        printf("server say: %s\n", buf.c_str());
    }
    client.Close();
    return 0;
}
