#include<iostream>
#include"httplib.h"
using namespace std;
void helloworld(const httplib::Request& req, httplib::Response& resp){
    cout << "method: " << req.method << endl;
    cout << "path: " << req.method << endl;
    cout << "body: " << req.body << endl;

    resp.status = 200;
    resp.body = "<html><body><h1><Hello World</h1></body></html>";
    resp.set_header("Content-Type", "text/html");
    resp.set_header("Content-Length", to_string(resp.body.size()));
    return;
}
int main(){
    httplib::Server server;
    server.Get("/", helloworld);

    server.listen("0.0.0.0", 9000);
    return 0;
}
