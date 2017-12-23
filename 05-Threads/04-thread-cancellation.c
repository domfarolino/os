#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3

void* thread_1() {
  printf("Thread 1 Starting!\n");
  sleep(1);
  printf("Thread 1 still working...\n");
  return NULL;
}

void* thread_2() {
  printf("Thread 2 Starting!\n");
  sleep(2);
  printf("Thread 2 still working...\n");
  return NULL;
}

void* thread_3() {
  printf("Thread 3 Starting!\n");
  sleep(3);
  printf("Thread 3 still working...\n");
  return NULL;
}

int main() {
  pthread_t threads[NUM_THREADS];
  void* (*threadHandlers[NUM_THREADS])(void*) = {thread_1, thread_2, thread_3};
  for (int i = 0; i < NUM_THREADS; ++i) {
    pthread_create(&threads[i], NULL /* default attrs */, threadHandlers[i], NULL);
  }

  printf("Main thread!\n");

  for (int i = 0; i < NUM_THREADS; ++i) {
    pthread_join(threads[i], NULL /* expect no return value */);
  }
  return 0;
}
