CXX=gcc

main:
	make 01-shm-open && make 01-shm-unlink && make 02-shm-producer && make 02-shm-consumer

01-shm-open:
	$(CXX) $@.c -o 01-shm-open

01-shm-unlink:
	$(CXX) $@.c -o 01-shm-unlink

02-shm-producer:
	$(CXX) $@.c -o producer

02-shm-consumer:
	$(CXX) $@.c -o consumer
