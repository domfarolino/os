#include <stdio.h>
#include <string.h> // for strlen()
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1
#define MAX_MSG_SIZE 128

int main() {
  int fd[2]; // where our pipe will reside

  if (pipe(fd) == -1) {
    fprintf(stderr, "Failed to create a pipe\n");
  }

  pid_t pid = fork();
  if (pid == -1) {
    fprintf(stderr, "Failed to fork a child process\n");
  }

  if (pid > 0) { // parent process
    close(fd[READ_END]); // close unused pipe end

    char messageBuffer[] = "This message originated from the parent and is destined for the child";

    printf("Press enter to send a message via the pipe to the child process\n");
    getchar();
    write(fd[WRITE_END], messageBuffer, strlen(messageBuffer));

    wait(NULL);
    printf("Parent process terminating...\n");
    close(fd[WRITE_END]);
  } else { // child process
    close(fd[WRITE_END]);

    char readBuffer[MAX_MSG_SIZE];
    read(fd[READ_END], readBuffer, MAX_MSG_SIZE);

    printf("  Read message '%s' from parent process via pipe\n", readBuffer);
    printf("  Child process terminating...\n\n");
    close(fd[READ_END]);
  }

  return 0;
}
