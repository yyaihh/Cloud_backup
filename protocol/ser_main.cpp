#include<iostream>
#include"socket_tcp.hpp"
using namespace std;

typedef struct{
    int num1;
    int num2;
    char c;
}data;

int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "Should input ./ser_main [ip] [port]\n";
        return -1;
    }
    TcpSocket ser;
    CHECK_RET(ser.Socket());
    CHECK_RET(ser.Bind(argv[1], atoi(argv[2])));
    CHECK_RET(ser.Listen());
    string ip;
    uint16_t port;
    TcpSocket newsock;
    while(ser.Accept(&newsock, &ip, &port) == false);
    printf("new connection:[ip: %s][port: %d]\n", ip.c_str(), port);
    while(1){
        data buf;
        int r = recv(newsock.GetFd(), &buf, sizeof(buf), 0);
        if(r == 0){
            break;
            cout << "connection break\n";
        }
        if(r < 0){
            break;
            perror("recv error");
        }
        if(buf.c == '+'){
            cout << buf.num1 << buf.c << buf.num2 << " = " << buf.num1 + buf.num2 <<endl;
        }
    }
    newsock.Close();
    ser.Close();
    return 0;
}
