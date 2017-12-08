#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
  int arg1;
  int arg2;
  int result;
} ThreadArgPackage;


void* threadedFunction(void* inputArgs) {
  ThreadArgPackage* threadArgs = (ThreadArgPackage*)inputArgs;

  // Compute some result and store it in the original argument package
  threadArgs->result = threadArgs->arg1 + threadArgs->arg2;
  return NULL; // implicitly calling pthread_exit
}

int main() {
  pthread_t threadId;
  ThreadArgPackage threadArgs;
  threadArgs.arg1 = 10;
  threadArgs.arg2 = 20;
  pthread_create(&threadId, NULL, threadedFunction, (void*)&threadArgs);
  pthread_join(threadId, NULL);
  printf("The results of the other thread's labors were: %d\n", threadArgs.result);
  return 0;
}
