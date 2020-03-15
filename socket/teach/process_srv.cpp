/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述：使用封装的tcpsocket类实现tcp服务端程序
*     1. 创建套接字
*     2. 绑定地址信息
*     3. 开始监听
*     while(1) {
*       4. 获取新连接---一个服务器不可能只跟一个客户端通信
*       5. 通过新连接接收数据
*       6. 通过新连接发送数据
*     } 
*     7. 关闭套接字
================================================================*/
#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "tcpsocket.hpp"

void sigcb(int signo) 
{
    //waitpid() > 0 表示有子进程退出--若返回值<=0则表示没有子进程退出了
    //WNOHANG--非阻塞操作
    //SIGCHLD信号是一个非可靠信号有可能丢失事件-因此循环处理到把已经退出的完全处理完毕
    while(waitpid(-1, NULL, WNOHANG) > 0);
}
int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("em: ./tcp_srv host_ip host_port\n");
        return -1;
    }
    //对SIGCHLD信号自定义处理，等到子进程退出的时候处理一下就可以
    //这样就避免了父进程一直等待的情况
    signal(SIGCHLD, sigcb);//自定义信号处理函数
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket lst_sock;
    CHECK_RET(lst_sock.Socket()); //创建套接字
    CHECK_RET(lst_sock.Bind(ip, port));//绑定地址信息
    CHECK_RET(lst_sock.Listen());//开始监听
    while(1) {
        TcpSocket newsock;
        std::string cli_ip;
        uint16_t cli_port;
        bool ret = lst_sock.Accept(&newsock,&cli_ip, &cli_port);//获取新连接
        if (ret == false) {
            continue;//服务端不会因为一次获取的失败而退出，而是继续重新获取下一个
        }
        printf("new conn:[%s:%d]\n", cli_ip.c_str(), cli_port);
        pid_t pid = fork();
        if (pid == 0) {
            //每个子进程负责与一个客户端进行循环通信
            while(1) {
                //------------------------------------------------------------------
                std::string buf;
                ret = newsock.Recv(&buf);//通过新连接接收数据
                if (ret == false) {newsock.Close(); continue;}
                printf("client say: %s\n", buf.c_str());
                //-----------------------------------------------------------------
                printf("server say:");//没有带换行，因此不会自动刷新标准输出缓冲区
                fflush(stdout); // 手动刷新标准输出缓冲区-将数据打印出来
                buf.clear();  //清空buf这个string对象的内容
                std::cin >> buf;
                ret = newsock.Send(buf);//通过新连接发送数据
                if (ret == false) {newsock.Close(); continue;}
            }
            newsock.Close();
            exit(0);
        }
        //注意：新的套接字创建在创建子进程之前，因此父子进程各自独有一份
        //父进程并不与客户端进行通信,因此直接关闭新的套接字-关闭的是自己的不影响子进程
        newsock.Close();
    }
    lst_sock.Close();
    return 0;
}

