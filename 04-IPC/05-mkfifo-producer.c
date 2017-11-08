#include <stdio.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main() {
  const char pipeName[] = "named-pipe";
  if (mkfifo(pipeName, 0755) < 0) {
    printf("There was an error creating fifo, errno: %d\n", errno);
    return 1;
  }

  printf("Opening named pipe with O_WRONLY...note this will block until the consumer opens the pipe\n");
  int pipeFd = open(pipeName, O_CREAT | O_WRONLY | O_TRUNC);

  const char message[] = "This is some content for the consumer";
  write(pipeFd, message, strlen(message));

  printf("Enter any key to close and unlink the fifo");
  getchar();
  close(pipeFd);
  unlink(pipeName);
  return 0;
}
