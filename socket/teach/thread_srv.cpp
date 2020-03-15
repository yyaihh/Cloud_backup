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
#include "tcpsocket.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("em: ./tcp_srv host_ip host_port\n");
        return -1;
    }
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
        pthread_t tid;
        //newsock传值操作会new一个新对象，是为了防止传地址，而变量是一个局部变量
        //循环完毕这个变量就被释放了，在线程中就无法使用了
        pthread_create(&tid, NULL, thr_start, (void*)newsock);//创建线程；
        pthread_detach(tid);
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
    lst_sock.Close();
    return 0;
}

