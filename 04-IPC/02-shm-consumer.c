#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/errno.h>
#include <unistd.h>

int main() {
  // Declare a name and a size in bytes for our shared memory
  printf("\x1B[34m");
  const char* NAME = "shared-memory";
  const int SIZE = 64;

  // 1.) Open the shared memory file descriptor, read only because
  // we're assuming the producer has set it up for us
  int shm_fd = shm_open("shared-memory", O_RDONLY, 0755);

  if (shm_fd >= 0) {
    printf("The shared memory object was opened successfully with the file descriptor %d\n", shm_fd);
  } else if (shm_fd == -1) {
    printf("Failed to open our shared memory file descriptor\n");
    printf("errno: %d\n", errno);
    return 1;
  }

  // 2.) Memory map the shared memory object file descriptor
  void* ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

  if (ptr >= 0) {
    printf("Successfully mapped our shared memory to the shared memory file descriptor. Address: %p\n", ptr);
  } else if (ptr == MAP_FAILED) { // check out it's definition
    printf("Failed to map the shared memory from another process to the shared memory file descriptor\n");
    printf("errno: %d\n", errno);
  }

  // 3.) Read from the shared memory and enjoy the fruits of our labor!
  printf("Here's the data in our shared memory:\n%s\n", (char*)ptr);

  // 4.) Unlink the shared memory file descriptor object from its name so we can reuse it later
  int sharedMemoryUnlinkResult = shm_unlink(NAME);

  if (sharedMemoryUnlinkResult == 0) {
    printf("Unlinked our shared memory object with its name so it can be reused in the future\n");
  } else if (sharedMemoryUnlinkResult == -1) {
    printf("Failed to unlink our shared memory object with its name...\n");
    printf("errno: %d\n", errno);
  }

  printf("\x1B[00m");
  return 0;
}
