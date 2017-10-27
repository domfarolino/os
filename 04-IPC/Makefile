CXX=gcc
PROGRAMS=01-shm-open 01-shm-unlink 02-shm-producer 02-shm-consumer

all: $(PROGRAMS)

01-shm-open: 01-shm-open.c
	$(CXX) $^ -o $@

01-shm-unlink: 01-shm-unlink.c
	$(CXX) $^ -o $@

02-shm-producer: 02-shm-producer.c
	$(CXX) $^ -o $@

02-shm-consumer: 02-shm-consumer.c
	$(CXX) $^ -o $@

clean:
	rm $(PROGRAMS)
