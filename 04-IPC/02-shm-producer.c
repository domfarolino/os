#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/errno.h>
#include <unistd.h>
#include <string.h> // for strlen

int main() {
  printf("\x1B[34m");
  // Declare a name and a size in bytes for our shared memory
  const char* NAME = "shared-memory";
  const int SIZE = 64;

  // 1.) Create a shared memory file descriptor
  int shm_fd = shm_open(NAME, O_CREAT | O_RDWR | O_EXCL, 0755);

  if (shm_fd >= 0) {
    printf("The shared memory object was created with file descriptor %d\n", shm_fd);
  } else if (shm_fd == -1) {
    printf("There was an error creating the shared memory object file descriptor (perhaps it already exists?)\n");
    printf("errno: %d\n", errno);
    return 1;
  }

  // 2.) Configure size of shared memory object
  int truncation = ftruncate(shm_fd, 64);

  printf("The file referenced by %d ", shm_fd);
  if (truncation == 0) {
    printf("has successfully been extended to size %d\n", SIZE);
  } else if (truncation == -1) {
    printf("has failed to be extended to size %d\n", SIZE);
    printf("errno: %d\n", errno);
    return 1;
  }

  // 3.) Map our shared memory file descriptor to a chunk of memory in our process's address space
  void* sharedMemoryPointer = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (sharedMemoryPointer >= 0) { // a valid memory address
    printf("A memory map for the shared memory has been created and starts at address: %p\n", sharedMemoryPointer);
  } else if (sharedMemoryPointer == MAP_FAILED) { // check out it's definition
    printf("Failed to create memory map to file descriptor\n");
    printf("errno: %d\n", errno);
    return 1;
  }

  const char* firstMessage = "Here's some text for our shared memory.";
  const char* secondMessage = ".. And SOME MORE!";
  printf("Writing to shared memory: \n%s%s\n", firstMessage, secondMessage);
  sprintf(sharedMemoryPointer, "%s", firstMessage);
  sprintf(sharedMemoryPointer + strlen(firstMessage), "%s", secondMessage);
  printf("\x1B[00m");
  return 0;
}
