CC=gcc
CXX=g++

all: fork fork2 fork3 forkChain

fork: fork.c
	$(CC) $^ -o $@

fork2: fork2.cpp
	$(CXX) $^ -o $@

forkChain: forkChain.cpp
	$(CXX) $^ -o $@

fork3: fork3.c
	$(CC) $^ -o $@

clean:
	rm fork forkChain fork2 fork3
