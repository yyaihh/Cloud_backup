.PHONY:all
all: named_pipe1 named_pipe2
named_pipe1:named_pipe1.c
	gcc -o $@ $^ -std=c99
named_pipe2:named_pipe2.c
	gcc -o $@ $^ -std=c99
	rm -f  my_named_pipe
