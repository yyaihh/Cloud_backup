#include<iostream>
#include<string>
#include"ser_udp.hpp"
using namespace std;

void test(const string& req, string* resp){
    cout << "client say: " << req << endl;
    cout << "server say: ";
    getline(cin, *resp);
}

int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "Should input: ./ser_main [ip] [port]\n";
        return -1;
    }
    UdpServer us(argv[1], atoi(argv[2]));
    us.Start(test);
    return 0;
}
