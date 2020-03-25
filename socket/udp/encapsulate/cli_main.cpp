#include<iostream>
#include<string>
#include"cli_udp.hpp"
using namespace std;

void test1(string* resp){
    cout << "client say:";
    getline(cin, *resp);
}
void test2(const string& req, string* resp){
    cout << "servers say: " << req << endl;
    test1(resp);
}

int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "Should input: ./cli_main [ip] [port]\n";
        return -1;
    }
    UdpClient uc(argv[1], atoi(argv[2]));
    uc.Start(test1, test2);
    return 0;
}
