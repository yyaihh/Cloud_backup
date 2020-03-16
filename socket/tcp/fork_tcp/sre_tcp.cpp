#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<sys/wait.h>
#include"socket_tcp.hpp"

/*1. 创建套接字
2. 绑定地址信息
3. 开始监听
while(1) {
    4. 获取新连接---一个服务器不可能只跟一个客户端通信
    5. 通过新连接接收数据
    6. 通过新连接发送数据
}
7. 关闭套接字*/

void sigcb(int signo){
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("输入错误\n");
        return -1;
    }
    signal(SIGCHLD, sigcb);
    TcpSocket ser;
    CHECK_RET(ser.Socket());
    string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    CHECK_RET(ser.Bind(ip, port));
    CHECK_RET(ser.Listen());
    while(1){
        TcpSocket newsock;
        string ip;
        uint16_t port;
        bool ret = ser.Accept(&newsock, &ip, &port);
        if(ret == false) { continue; }//失败继续获取下一个
        cout << "建立新链接,ip:" << ip << "端口号:" << port << endl;
        pid_t pid = fork();
        if(pid < 0){
            perror("fork error");
            return -1;
        }
        if(pid == 0){
            string buf;
            while(1){
                bool ret_ = newsock.Recv(&buf);
                if (ret_ == false) { 
                    newsock.Close();
                    ser.Close();
                    exit(0);
                }
                printf("ip[%s]:port[%d]: %s\n", ip.c_str(), port, buf.c_str());
                cout << "serves say: "; 
                getline(cin, buf);
                ret = newsock.Send(buf);
            }
            newsock.Close();
        }
    }
    ser.Close();
    return 0;
}
