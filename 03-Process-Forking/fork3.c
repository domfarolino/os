#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  printf("Parent process: %d\n", getpid());
  pid_t firstChild = fork();

  // Child just dies right off the bat
  if (firstChild == 0) {
    printf("First child process: %d going into zombie state\n", getpid());
    exit(0);
  }

  // If in parent process catch signals that terminate or stop children
  if (firstChild) {
    printf("Parent process sleeping for a single second then waiting for terminated child\n");
    sleep(60);
    int status;
    pid_t waitedChild = waitpid(firstChild, &status, WNOHANG);

    if (waitedChild == 0) {
      printf("We tried to reap a process however none were available\n");
      return 0;
    }

    printf("Just waited for child %d ", waitedChild);
    if (WIFEXITED(status)) {
      printf("was terminated with exit status %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
      printf("was terminated with signal %d\n", WTERMSIG(status));
    } else if (WIFSTOPPED(status)) {
      printf("was stopped via stop signal: %d\n", WSTOPSIG(status));
    }
  }

  return 0;
}
