#include<iostream>
#include"socket_tcp.hpp"
using namespace std;
struct data{
    int num1;
    int num2;
    char c;
};

int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "Should input ./ser_main [ip] [port]\n";
        return -1;
    }
    TcpSocket cli;
    CHECK_RET(cli.Socket());
    CHECK_RET(cli.Connect(argv[1], atoi(argv[2])));
    while(1){
        data buf;
        cout << "请依次输入num1, num2\n";
        cin >> buf.num1;
        cin >> buf.num2;
        buf.c ='+';
        size_t slen = 0, ret = 0;
        size_t size = sizeof(buf);
        while(slen < size) {
            ret = send(cli.GetFd(), &buf + slen, size - slen, 0);
            if(ret < 0){
                perror("send error");
                break;
            }
            slen += ret;                              
        }
    }
    cli.Close();
    return 0;
}
