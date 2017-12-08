#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* threadStartsHere(void* threadArg) {
  printf("\x1B[34mOther thread: Inside another thread, brb going to sleep for a second\n");
  sleep(1);
  /**
   * By now, the argument has gone out of the
   * thread caller's scope, so dereferencing the
   * pointer will be NO GOOD.
   */
  int argCastResult = *(int*)(threadArg);
  printf("\x1B[34mOther thread: Alright, going to try take a look at our thread argument. Looks like we got arg: %d\n", argCastResult);
  return NULL;
}

/**
 * We use a function other than main() to create a thread
 * in this example so that the argument we pass into the
 * thread's function gets popped off the stack once the
 * thread has been created. Then later on (when the thread
 * finishes sleeping), access to its argument is undefined
 * (bc it doesn't exist in memory!)
 */
pthread_t createThread() {
  printf("\x1B[00mMain thread > createThread(): Inside function where we'll create a new thread whose argument is a local variable\n");
  int threadArgument = 25; // argument we want thread to have
  pthread_t threadId;
  pthread_create(&threadId, NULL, threadStartsHere, (void*)&threadArgument);
  printf("\x1B[00mMain thread > createThread(): New thread created successfully and given arg: %d. Returning back to main() now\n", threadArgument);
  return threadId;
}

int main() {
  printf("\x1B[00mMain thread: In main thread, going to create a new thread\n");
  pthread_t threadCreated = createThread();
  printf("\x1B[00mMain thread: Uh oh! Now the local variable which was used as the created thread's argument no longer exists!\n");
  pthread_join(threadCreated, NULL);
  printf("\x1B[00mMain thread: Ok, all done!!\n");
  return 0;
}
