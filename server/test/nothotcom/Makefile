all:test
test:test.cpp cloud_backup.hpp
	g++ $^ -o $@ -std=c++11 -lz -pthread -lboost_filesystem -lboost_system
