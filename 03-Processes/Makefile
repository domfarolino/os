CC=gcc
CPP=g++

all: fork fork2 fork3 forkChain

fork: fork.c
	$(CC) $^ -o $@

fork2: fork2.cpp
	$(CPP) $^ -o $@

forkChain: forkChain.cpp
	$(CPP) $^ -o $@

fork3: fork3.c
	$(CC) $^ -o $@

clean:
	rm fork forkChain fork2 fork3
