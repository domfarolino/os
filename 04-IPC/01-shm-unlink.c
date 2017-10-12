#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/errno.h>
#include <unistd.h>

int main() {
  // 1.) In another process we can do something we couldn't really do before, i.e. begin to
  // tamper with stuff another process setup.
  int unlinkResult = shm_unlink("shared-memory");
  if (unlinkResult == -1) {
    printf("Failed to unlink shared memory file descriptor with its name. Failed with errno: %d\n", errno);
  } else {
    printf("Shared memory file descriptor successfully unlinked with its name!\n");
  }
  return 0;
}
