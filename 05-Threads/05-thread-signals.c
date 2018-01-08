#include <stdio.h>
#include <string.h> // for memset
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

void signalHandler(int signal) {
  printf(" SIGINT signal handler called! (wait 8 seconds)\n");
  sleep(8);
}

void addSignalHandler(const char* threadName) {
  struct sigaction actionStruct;
  memset(&actionStruct, 0, sizeof actionStruct);
  actionStruct.sa_handler = signalHandler;
  actionStruct.sa_flags = 0;
  sigemptyset(&actionStruct.sa_mask);
  /*struct sigaction {
    void     (*sa_handler)(int);
    void     (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t   sa_mask;
    int        sa_flags;
  };*/

  if (sigaction(SIGINT, &actionStruct, NULL) == -1) {
    printf("Cannot install signal handler, errno: %d\n", errno);
  }

  while (1) {
    printf("%s waiting for SIGINT\n", threadName);
    sleep(1);
  }
}

void* thread_1() {
  addSignalHandler("Thread 1");
  return NULL;
}

void* thread_2() {
  addSignalHandler("Thread 2");
  return NULL;
}

int main() {
  printf("This progam has an infinite loop, allowing you to see which threads are available to catch signals, and when. When you are done, press CTLR+\\ to quit\n\n");
  pthread_t tid1, tid2;
  pthread_create(&tid1, NULL, thread_1, NULL);
  pthread_create(&tid2, NULL, thread_2, NULL);
  addSignalHandler("Main thread");
  while (1) {
    printf("Main thread waiting for SIGINT\n");
    sleep(1);
  }

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  return 0;
}
