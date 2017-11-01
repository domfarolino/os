#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  int length;
  std::cout << "Please enter the number of processes to fork:" << std::endl;
  if (length < 0) length = 4; // sure why not
  std::cin >> length;
  pid_t *arr = new pid_t[length];

  // Initialize to 0, a clever bit so we can make a linear hierarchy of descendants
  for (int i = 0; i < length; ++i) arr[i] = 1;

  std::cout << "Hi I'm " << getpid() << ", my parent is " << getppid() << std::endl;
  arr[0] = fork();
  if (arr[0]) waitpid(arr[0], NULL, 0);

  for (int i = 0; i < length; ++i) {
    if (arr[i] == 0) {
      std::cout << std::string(i + 1, ' ') << "Hi I'm " << getpid() << ", my parent is " << getppid() << std::endl;
      if (i + 1 < length) { // Only fork new processes if we're not the ultimate child
        arr[i+1] = fork();
        // We want to wait on the child process we just spawned
        if (arr[i+1]) waitpid(arr[i + 1], NULL, 0);
      }
    }
  }

  delete[] arr;
  return 0;
}
