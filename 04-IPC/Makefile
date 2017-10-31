CXX=gcc
PROGRAMS=01-shm-open 01-shm-unlink 02-shm-producer 02-shm-consumer 03-mmap-file
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
  CFLAGS= -lrt
else
  CFLAGS=
endif

all: $(PROGRAMS)

01-shm-open: 01-shm-open.c
	$(CXX) $^ -o $@ $(CFLAGS)

01-shm-unlink: 01-shm-unlink.c
	$(CXX) $^ -o $@ $(CFLAGS)

02-shm-producer: 02-shm-producer.c
	$(CXX) $^ -o $@ $(CFLAGS)

02-shm-consumer: 02-shm-consumer.c
	$(CXX) $^ -o $@ $(CFLAGS)

03-mmap-file: 03-mmap-file.c
	$(CXX) $^ -o $@ $(CFLAGS)

clean:
	rm $(PROGRAMS)
