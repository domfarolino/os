#include <stdio.h>
#include <sys/mman.h>
#include <sys/errno.h>
#include <fcntl.h>

int main() {
  // 1.) Create a shared memory object that the kernel can reference by a given name
  int shmOpenResult = shm_open("shared-memory", O_CREAT | O_RDWR | O_EXCL);
  if (shmOpenResult == -1) {
    printf("Failed to create shared memory file descriptor, failed with errno: %d\n", errno);
  } else {
    printf("Shared memory file descriptor created!\n");
  }

  // 2.) Optionally unlink it here
  /*int shmUnlinkResult = shm_unlink("shared-memory");
  if (shmUnlinkResult == -1) {
    printf("Failed to unlink shared memory file descriptor with its name. Failed with errno: %d\n", errno);
  } else {
    printf("Shared memory file descriptor unlinked with its name!\n");
  }*/
  return 0;
}
