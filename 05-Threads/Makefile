CXX=gcc
PROGRAMS=01-pthread-example 02-thread-arg-scope 03-thread-struct-args 04-thread-cancellation
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
  CFLAGS= -lpthread # To use the pthread library
else
  CFLAGS=
endif

all: $(PROGRAMS)

01-pthread-example: 01-pthread-example.c
	$(CXX) $^ -o $@ $(CFLAGS)

02-thread-arg-scope: 02-thread-arg-scope.c
	$(CXX) $^ -o $@ $(CFLAGS)

03-thread-struct-args: 03-thread-struct-args.c
	$(CXX) $^ -o $@ $(CFLAGS)

04-thread-cancellation: 04-thread-cancellation.c
	$(CXX) $^ -o $@ $(CFLAGS)
clean:
	rm $(PROGRAMS)
