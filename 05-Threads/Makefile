CC=g++
PROGRAMS=01-thread-intro 02-data-races 03-producer-consumer-queue 04-producer-consumer-buffer 05-producer-consumer-queue-posix
UNAME := $(shell uname)

CFLAGS = -std=c++14 -g

ifeq ($(UNAME), Linux)
  CFLAGS+= -lpthread # To use the pthread library
else
  CFLAGS+=
endif

all: $(PROGRAMS)

01-thread-intro: 01-thread-intro.cc
	$(CC) $^ -o $@ $(CFLAGS)

02-data-races: 02-data-races.cc
	$(CC) $^ -o $@ $(CFLAGS)

03-producer-consumer-queue: 03-producer-consumer-queue.cc
	$(CC) $^ -o $@ $(CFLAGS)

04-producer-consumer-buffer: 04-producer-consumer-buffer.cc
	$(CC) $^ -o $@ $(CFLAGS)

05-producer-consumer-queue-posix: 05-producer-consumer-queue-posix.cc
	$(CC) $^ -o $@ $(CFLAGS)
clean:
	rm $(PROGRAMS)
