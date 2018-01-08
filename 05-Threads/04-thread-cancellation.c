#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3

void* thread_1() {
  printf("Thread 1: Starting! I have a deferred cancellation type\n");
  // Use a manual sleep because the sleep() system call creates a cancellation point
  time_t endTime = time(NULL) + 1;
  while(time(NULL) < endTime) {}

  printf("\nThread 1: Still working...I'll create a cancellation point in two seconds\n");
  endTime = time(NULL) + 2;
  while(time(NULL) < endTime) {}

  pthread_testcancel();
  return NULL;
}

void* thread_2() {
  printf("Thread 2: Starting! I can be cancelled in an asynchronous manner\n");
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  sleep(3); // main thread will cancel this thread sometime during our 3 second sleep
  return NULL;
}

void* thread_3() {
  printf("Thread 3: Starting! I cannot be cancelled right now\n");
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
  sleep(4);

  printf("Thread 3: Ok going to enable asynchronous cancellation in two seconds\n");
  sleep(2);

  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  return NULL;
}

int main() {
  pthread_t threads[NUM_THREADS];
  void* (*threadHandlers[NUM_THREADS])(void*) = {thread_1, thread_2, thread_3};

  // Create threads
  for (int i = 0; i < NUM_THREADS; ++i) {
    pthread_create(&threads[i], NULL /* default attrs */, threadHandlers[i], NULL);
  }

  printf("Main thread: Ok, going to send a cancellation request to 'Thread 1'\n");
  pthread_cancel(threads[0]);
  pthread_join(threads[0], NULL);

  printf("Main thread: Ok 'Thread 1' has stopped! Sending cancellation request 'Thread 2'\n");
  pthread_cancel(threads[1]);
  pthread_join(threads[1], NULL);

  printf("Main thread: Ok 'Thread 2' has stopped asynchronously! Sending cancellation request to 'Thread 3'\n");
  pthread_cancel(threads[2]);
  pthread_join(threads[2], NULL);

  printf("Main thread: Ok 'Thread 3' has stopped as well! All finished here, peace out\n");
  return 0;
}
