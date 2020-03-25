all:ser_main cli_main
ser_main:ser_main.cpp ser_udp.hpp socket_udp.hpp
		g++ $^ -o $@ -std=c++11
cli_main:cli_main.cpp cli_udp.hpp socket_udp.hpp
		g++ $^ -o $@ -std=c++11
