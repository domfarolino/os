#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/errno.h>

int main(int argc, char** args) {
  const int SIZE = 128;
  printf("In this demo we're going to write to a file by mapping it to\nmemory in the process's address space, and writing to that memory.\n\n");
 
  int fd = open("03-input", O_RDWR | O_CREAT, 0755);
  if (fd == -1) {
    printf("There was an error opening up 'input', errno: %d\n", errno);
    return 1;
  }

  printf("Successfully opened file `03-input`. Please input a string\nthat will be written to the memory-mapped file.\n");

  char input[SIZE];
  fgets(input, SIZE, stdin);
  int inputSize = strlen(input) + 1; // don't forget '\0'

  printf("Now about to call ftruncate on `03-input`'s file descriptor to extend/shrink the\nfile's size to %d bytes (newline + null terminator). Press enter to continue.\n", inputSize);
  getchar();

  ftruncate(fd, inputSize);

  printf("Next we'll go ahead and map the file to memory in the process. We can either map it as\nMAP_SHARED or MAP_PRIVATE, which basically means writes will either be propagated to the\noriginal file or private to the process's memory (copy-on-write). Press any key to continue.\n");
  getchar();

  void* ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FILE, fd, 0);
  char* charPtr = (char*)ptr;
  if (ptr == MAP_FAILED) { // check out the definition of MAP_FAILED! (sys/mman.h)
    close(fd);
    printf("Error mapping the file to process memory, errno: %d\n", errno);
    return 1;
  }

  printf("The file has been successfully mapped to memory and starts at address: %p.\nPress any key to continue.\n", ptr);
  getchar();

  for (int i = 0; i < inputSize; ++i) {
    printf("Writing %c to position %d\n", input[i], i);
    charPtr[i] = input[i];
  }

  if (msync(charPtr, SIZE, MS_SYNC) == -1) {
    printf("Error syncing file to disk\n");
  }

  munmap(ptr, SIZE);
  close(fd);

  printf("Alright! Now go ahead and check the file's contents however you'd like, and verify that\nthe contents have been replaced by the above message.\n");

  return 0;
}
