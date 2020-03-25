#include<iostream>
#include<string>
#include"ser_tcp.hpp"
using namespace std;

void test(const string& req, string* resp, const string& ip, const uint16_t port){
    printf("client[ip:%s]:[port:%d]: %s\n", ip.c_str(), port, req.c_str());
    cout << "servers say:";
    getline(cin, *resp);
}

int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "Should input ./ser_main [ip] [port]\n";
        return -1;
    }
    TcpThreadServer ts(string(argv[1]), atoi(argv[2]));
    ts.Start(test);
    return 0;
}
