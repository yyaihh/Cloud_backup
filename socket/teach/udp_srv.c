/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述：这个demo用于体会udp服务端通信的编程流程以及接口使用 
*       1. 创建套接字
*       2. 为套接字绑定地址信息
*       3. 接收数据
*       4. 发送数据
*       5. 关闭套接字
================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>//struct sockaddr_in结构体的定义以及协议的宏
#include <arpa/inet.h>//一些字节序转换的接口
#include <sys/socket.h> // 套接字接口

int main(int argc, char *argv[])
{
    //通过参数传入服务端要绑定的地址信息（IP地址和端口）
    if (argc != 3) {
        printf("em: ./udp_srv 192.168.122.132 9000\n");
        return -1;
    }
    //./udp_srv 192.168.122.132 9000  在命令行通过这句话运行程序
    //其中argv[0]=./udp_srv  argv[1]=192.168.122.132  argv[2]=9000
    const char *ip_addr = argv[1];//通过参数获取IP地址
    uint16_t port_addr = atoi(argv[2]);//通过参数获取端口数据

    //创建套接字
    //socket(地址域， 套接字类型， 协议类型) 
    //这一步就注明了本次通信是什么通信 AF_INET-IPV4的通信,并且是UDP协议的数据报传输服务
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if  (sockfd < 0) {
        perror("socket error");
        return -1;
    }
    //定义IPv4地址结构
    //htons  将无符号两个字节的数据转换为网络字节序的数据
    //inet_pton - 将字符串IP地址转换为网络字节序IP地址
    struct sockaddr_in addr;//定义一个IPV4地址结构
    addr.sin_family = AF_INET;//这个成员用于标识这个结构是一个什么样的地址结构
    //htons(无符号2个字节的主机字节序数据)
    addr.sin_port = htons(port_addr);//网络字节序端口,使用htons将主机字节序数据转换一下
    //inet_pton(地址域类型，字符串IP地址， 接收转换后数据的缓冲区)
    inet_pton(AF_INET, ip_addr, &addr.sin_addr.s_addr);//将一个字符串IP地址转换为网络字节序IP地址
    //绑定地址信息
    int ret ;
    socklen_t len = sizeof(struct sockaddr_in);
    //bind(描述符， 要绑定的地址信息， 地址信息长度)
    ret = bind(sockfd, (struct sockaddr*)&addr, len);
    if (ret < 0) {
        perror("bind error");
        return -1;
    }
    //循环接收/发送数据
    while(1) {
        //作为服务端应该先接收数据
        char buf[1024] = {0};//用于存放对端发送的数据
        struct sockaddr_in cliaddr;//用于存放对端地址信息
        len = sizeof(struct sockaddr_in);//用于存放地址信息长度
        //recvfrom(描述符,缓冲区,长度,参数,对端地址存放缓冲区,地址长度缓冲区);
        //参数=0 指的是阻塞接收数据--若缓冲区中没有数据则需要等待，直到有数据
        ret = recvfrom(sockfd, buf, 1023, 0, (struct sockaddr*)&cliaddr, &len);
        if (ret < 0) {
            perror("recvfrom error");
            close(sockfd);
            return -1;
        }
        char cli_ip[32] = {0};
        uint16_t cli_port = ntohs(cliaddr.sin_port);
        //将网络字节序数据转换为字符串IP地址
        inet_ntop(AF_INET, &cliaddr.sin_addr, cli_ip, 32);
        printf("client[%s:%d] say: %s\n", cli_ip, cli_port, buf);

        printf("server say: ");
        fflush(stdout);
        memset(buf, 0x00, 1024);//初始化buf缓冲区为0
        scanf("%s", buf);//从标准输入获取数据放到buf中，然后响应给客户端


        //sendto(描述符， 要发送的数据，数据长度，参数，对端地址， 地址长度)
        //参数=0--标识阻塞发送，数据放不到缓冲区（缓冲区满了），则等待
        ret=sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&cliaddr, len);
        if (ret < 0) {
            perror("sendto error");
            close(sockfd);
            return -1;
        }
    }
    close(sockfd);
    return 0;
}
