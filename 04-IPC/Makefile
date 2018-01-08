CC=gcc
PROGRAMS=01-shm-open 01-shm-unlink 02-shm-producer 02-shm-consumer 03-mmap-file 04-pipe 05-mkfifo-producer 05-mkfifo-consumer
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
  CFLAGS= -lrt # To use shm_get and shm_unlink in Linux we must link with the Realtime Extensions Library
else
  CFLAGS=
endif

all: $(PROGRAMS)

01-shm-open: 01-shm-open.c
	$(CC) $^ -o $@ $(CFLAGS)

01-shm-unlink: 01-shm-unlink.c
	$(CC) $^ -o $@ $(CFLAGS)

02-shm-producer: 02-shm-producer.c
	$(CC) $^ -o $@ $(CFLAGS)

02-shm-consumer: 02-shm-consumer.c
	$(CC) $^ -o $@ $(CFLAGS)

03-mmap-file: 03-mmap-file.c
	$(CC) $^ -o $@ $(CFLAGS)

04-pipe: 04-pipe.c
	$(CC) $^ -o $@ $(CFLAGS)

05-mkfifo-producer: 05-mkfifo-producer.c
	$(CC) $^ -o $@ $(CFLAGS)

05-mkfifo-consumer: 05-mkfifo-consumer.c
	$(CC) $^ -o $@ $(CFLAGS)


clean:
	rm $(PROGRAMS)
