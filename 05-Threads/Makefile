CC=gcc
PROGRAMS=01-pthread-example 02-thread-arg-scope 03-thread-struct-args 04-thread-cancellation 05-thread-signals
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
  CFLAGS= -lpthread # To use the pthread library
else
  CFLAGS=
endif

all: $(PROGRAMS)

01-pthread-example: 01-pthread-example.c
	$(CC) $^ -o $@ $(CFLAGS)

02-thread-arg-scope: 02-thread-arg-scope.c
	$(CC) $^ -o $@ $(CFLAGS)

03-thread-struct-args: 03-thread-struct-args.c
	$(CC) $^ -o $@ $(CFLAGS)

04-thread-cancellation: 04-thread-cancellation.c
	$(CC) $^ -o $@ $(CFLAGS)

05-thread-signals: 05-thread-signals.c
	$(CC) $^ -o $@ $(CFLAGS)
clean:
	rm $(PROGRAMS)
