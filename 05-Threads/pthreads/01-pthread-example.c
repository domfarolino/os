#include <stdio.h>
#include <stdlib.h> // for atoi
#include <pthread.h>

// Global sum shared between threads
int sum = 0;

/**
 * This function will be run on a separate thread.
 * It's goal is to compute the sum of the first N
 * numbers and return its result.
 */
void* threadRunner(void *input) {
  int n = *(int*)input;
  printf("Printing from another thread, and our argument (n) = %d\n", n);
  for (int i = 1; i <= n; ++i) {
    sum += i;
  }

  pthread_exit(0);
}

int main(int argc, char* argv[]) {
  // Thread identifier and attributes
  pthread_t threadId;
  pthread_attr_t threadAttributes;

  // Some basic error checking
  if (argc != 2) {
    printf("Usage: ./<binary> integerValue\n");
    return 1;
  }

  if (atoi(argv[1]) < 1) {
    printf("The argument must be an integer\n");
    return 1;
  }

  int threadArg = atoi(argv[1]);

  // Get default thread attributes
  pthread_attr_init(&threadAttributes);

  // Create the thread
  printf("Creating and starting a new thread!\n");
  pthread_create(&threadId, &threadAttributes, threadRunner, (void*)&threadArg);

  // Wait for and join with child thread
  pthread_join(threadId, NULL);
  printf("The resulting sum from our off-main-thread computation is: %d\n", sum);
  return 0;
}
