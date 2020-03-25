all:ser_main cli_main
ser_main:ser_main.cpp ser_tcp.hpp socket_tcp.hpp
		g++ $^ -o $@ -std=c++11
cli_main:cli_main.cpp cli_tcp.hpp socket_tcp.hpp
		g++ $^ -o $@ -std=c++11
