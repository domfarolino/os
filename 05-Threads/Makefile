CC=g++
PROGRAMS=01-thread-intro
UNAME := $(shell uname)

CFLAGS = -std=c++11

ifeq ($(UNAME), Linux)
  CFLAGS+= -lpthread # To use the pthread library
else
  CFLAGS+=
endif

all: $(PROGRAMS)

01-thread-intro: 01-thread-intro.cc
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm $(PROGRAMS)
