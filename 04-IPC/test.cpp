#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/errno.h>

int main() {
  const int SIZE = 32;
  int fd = open("03-input", O_RDWR | O_CREAT, 0755);
  if (fd == -1) {
    printf("There was an error opening up 'input', errno: %d\n", errno);
    return 1;
  }

  std::cout << "Opened file 03-input. File should be 24 bytes, and we're going to call\n" <<
                "ftruncate on it to either truncate or expand (in this case, expand) it to\n" <<
                "64 bytes. Enter any character to continue." << std::endl;
  std::cin.get();

  ftruncate(fd, SIZE);
  std::cout << "Now go ahead and in another terminal window check with `l` to see that the file\n" <<
                "is 32 bytes in size. Now we'll read the file via the file descriptor an the `read`\n" <<
                "system call with a buffer, two bytes at a time. Press any key to continue." << std::endl;
  std::cin.get();

  char buffer[SIZE];
  int bytesRead;
  while ((bytesRead = read(fd, buffer, 2)) > 0) {
    printf("The data read to the buffer is: %s\n", buffer);
  }

  std::cout << "Next we'll go ahead and map the file to memory in the process. We can either map it as\n" <<
                "MAP_SHARED or MAP_PRIVATE, which basically means writes will either be propagated to the\n" <<
                "original file or private to the process (copy-on-write). Press any key to continue." << std::endl;
  std::cin.get();

  void* ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE| MAP_FILE, fd, 0);
  char* charPtr = (char*)ptr;
  if (ptr == MAP_FAILED) { // check out the definition of MAP_FAILED! (sys/mman.h)
    close(fd);
    printf("Error mapping the file to process memory, errno: %d\n", errno);
    return 1;
  }

  std::cout << "The file has been successfully mapped to memory and starts at address: \n" << 
                ptr << ". Now go ahead and enter a string to write to the address, and we'll\n" <<
                "see that the changes are propagated to the actual file in the directory." << std::endl;
  char input[SIZE];
  std::cin.getline(input, SIZE);
  for (int i = 0; i < SIZE; ++i) {
    printf("Writing %c to position %d\n", input[i], i);
    charPtr[i] = input[i];
  }

  if (msync(charPtr, SIZE, MS_SYNC) == -1) {
    printf("Error syncing file to disk\n");
  }

  munmap(ptr, SIZE);
  close(fd);

  std::cout << "Alright! Now go ahead and check the file's contents however you'd like, and verify that\n" <<
                "the contents have been replaced by the above message." << std::endl;
  return 0;
}
