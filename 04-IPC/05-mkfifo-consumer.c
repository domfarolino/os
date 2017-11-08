#include <stdio.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 128

int main() {
  const char pipeName[] = "named-pipe";
  char buffer[BUFFER_SIZE];

  int pipeFd = open(pipeName, O_RDONLY, 0755);

  if (pipeFd == -1) {
    printf("Error opening file, errno: %d\n", errno);
    return 1;
  }

  int bytesRead;
  while (1) {
    printf("Enter any key to check to see if producer has written message to us...");
    getchar();
    bytesRead = read(pipeFd, buffer, BUFFER_SIZE);
    printf("bytesRead: %d\n", bytesRead);
    if (bytesRead == -1) {
      printf("There was an error reading, errno: %d\n", errno);
      return 1;
    }
    if (bytesRead) break;
  }

  printf("The producer's message is: %s\n", buffer);
  printf("Enter any key to close the fifo");
  getchar();
  close(pipeFd);
  return 0;
}
