all:ser_tcp cli_tcp
ser_tcp:ser_tcp.cpp socket_tcp.hpp
		g++ $^ -o $@ -std=c++11 -pthread
cli_tcp:cli_tcp.cpp socket_tcp.hpp
		g++ $^ -o $@ -std=c++11
