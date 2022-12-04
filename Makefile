CC=g++
CFLAGS=-Wall -g

all:
	$(CC) $(CFLAGS) sources/main.cpp sources/tasks.cpp sources/s300.cpp -o sim
run:
	./sim $(ARGS)
