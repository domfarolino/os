#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  printf("In parent process with pid: %d\n", getpid());
  pid_t firstFork = fork();
  if (firstFork == 0) {
    printf("  In child process with pid: %d and parent: %d\n", getpid(), getppid());
  } else {
    printf("In parent process with pid: %d\n", getpid());
  }

  /**
   * Remember the following line (which creates another process) gets called from two
   * processes, the parent and the child. This means in one case we'll be creating a new
   * child of the original parent, and in another we'll be creating a grandchild process
   * (a child from a child).
   */
  pid_t secondFork = fork();
  if (firstFork && secondFork == 0) {
    printf("  In child process with pid: %d and parent: %d\n", getpid(), getppid());
  } else if (secondFork == 0 && firstFork == 0) {
    printf("    In grandchild process with pid: %d and parent: %d\n", getpid(), getppid());
  }

  int status1, status2;

  // Parent should wait for its two children
  // If we do no waiting, the parent may finish
  // executing before its children forcing the child
  // processes to be orphaned and adopted by init
  if (firstFork && secondFork) {
    waitpid(firstFork, &status1, 0);
    waitpid(secondFork, &status2, 0);
  }

  // Child process should wait on its child
  if (firstFork == 0 && secondFork) {
    waitpid(secondFork, &status1, 0);
  }

  return 0;
}
