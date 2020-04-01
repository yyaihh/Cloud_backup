#include<iostream>
#include"cloud_backup.hpp"

int main() {
	CloudClient cl("./", "192.168.40.128", 9000);
	cl.Start();
	return 0;
}