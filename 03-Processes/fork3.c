#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  printf("Parent process: %d\n", getpid());
  pid_t firstChild = fork(), secondChild = 0;
  if (firstChild) secondChild = fork();

  // First child dies right off the bat and
  // becomes a zombie process
  if (firstChild == 0) {
    printf("First child process: %d exiting and going into a zombie state\n", getpid());
    exit(0);
  }

  // Second child pauses and waits for termination (or stop) signal
  if (firstChild && secondChild == 0) {
    printf("Second child process: %d pauses and waits for signal\n", getpid());
    pause();
  }

  /**
   * In the parent process, first try and reap any zombie processes without blocking
   * on termination (WNOHANG), and then block on the termination or stopping of another
   * process.
   */
  if (firstChild && secondChild) {
    printf("Parent process sleeping for a single second to make sure first child is in zombie state\n");
    sleep(1);
    int status;
    pid_t possibleZombieChild = waitpid(-1, &status, WNOHANG); // will catch firstChild in zombie state

    if (possibleZombieChild) {
      printf("We successfully reaped zombie child process %d\n", possibleZombieChild);
    } else {
      printf("We tried to reap a process however there were no zombie processes to claim!\n");
    }

    /**
     * Now we'll wait for our second child process to either
     * exit, or be terminated or stopped by a signal.
     */
    printf("Parent process will now wait for second child to either exit normally or be terminated (or stopped) by a signal.\n");
    pid_t waitedChild = waitpid(secondChild, &status, WUNTRACED);

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
