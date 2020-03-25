all:ser_udp cli_udp
ser_udp:ser_udp.cpp socket_udp.hpp
	g++ $^ -o $@ -std=c++11
cli_udp:cli_udp.cpp socket_udp.hpp
	g++ $^ -o $@ -std=c++11
