#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<unistd.h>
#include<cstring>
#include<netinet/in.h>//struct sockaddr_in 结构体以及协议的宏
#include<arpa/inet.h>//字节序转换的接口
#include<sys/socket.h>
using namespace std;
int main(int argc, char* argv[]){//通过参数传入服务端需要绑定的地址信息, 也就是IP地址和端口
    //传入的是服务端自己要绑定的地址
    if(argc != 3){

    }
    const char* ip_addr = argv[1];//获取IP地址
    uint16_t port_addr = atoi(argv[2]);//获取端口, 主机字节序
    //创建套接字
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//SOCK_DGRAM,是数据报
    if(sockfd < 0){
        perror("socket error");
        return -1;
    }
    //定义IPV4地址结构
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;//IPV4
    addr.sin_port = htons(port_addr);//将端口信息绑定
    inet_pton(AF_INET, ip_addr, &addr.sin_addr.s_addr);
    //绑定地址信息
    socklen_t len = sizeof(struct sockaddr);
    int ret = bind(sockfd, (struct sockaddr*)&addr, len);
    if(ret < 0){
        perror("bind error");
        return -1;
    }
    while(1){
        //服务端先接受数据
        char buf[1024] = { 0 };
        struct sockaddr_in cliaddr;
        len = sizeof(struct sockaddr_in);
        ret = recvfrom(sockfd, buf, 1023, 0, (struct sockaddr*)&cliaddr, &len);
        if(ret < 0){
            perror("recvfrom error");
            return -1;
        }
        cout << "女朋友说 :" << buf << endl;

        //回复客户端, 也就是发送数据
        cout << "请输入: ";
        memset(buf, 0, 1024);
        cin >> buf;
        ret = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&cliaddr, len);
        if(ret < 0){
            perror("sendto error");
            return -1;
        }
    }
    close(sockfd);
    return 0;
}
