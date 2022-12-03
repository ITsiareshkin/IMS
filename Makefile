CC=g++
CFLAGS=-Wall -g

all:
	$(CC) $(CFLAGS) sources/main.cpp sources/tasks.cpp sources/s300.cpp -o sim

# all:
# 	simulation

# simulation:
# 	$(CC) main.o tasks.o -o simulation

# main.o: main.cpp
# 	$(CC) $(CFLAGS) main.cpp
