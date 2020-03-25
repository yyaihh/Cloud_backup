/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述：通过封装的tcpsocket类实现一个tcp客户端程序
*       1. 创建套接字
*       2. 绑定地址信息（不推荐）
*       3. 向服务端发起连接请求
*       while(1) {
*           4. 先发送数据
*           5. 接收数据
*       } 
*       6. 关闭套接字
================================================================*/
#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include "tcpsocket.hpp"


void sigcb(int signo)
{
    printf("连接断开收到SIGPIPE信号\n");
}
int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("em: ./tcp_cli srv_ip srv_port\n");
        return -1;
    }
    signal(SIGPIPE, sigcb);
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket cli_sock;
    CHECK_RET(cli_sock.Socket());//创建套接字
    CHECK_RET(cli_sock.Connect(ip, port));//向服务端发起连接
    while(1) {
        std::cout << "client say:";
        std::string buf;
        std::cin >> buf;
        cli_sock.Send(buf);//当前客户端收发数据出错直接退出--

        buf.clear();
        cli_sock.Recv(&buf);//正常情况下应该关闭套接字然后重新建立连接
        std::cout << "server say: " << buf << std::endl;
    }
    cli_sock.Close();
    return 0;
}
