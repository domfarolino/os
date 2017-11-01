#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  std::cout << "In parent process, going to fork child now and wait for it!" << std::endl;
  pid_t childProcess = fork();

  if (childProcess) {
    int status;
    wait(&status);
    std::cout << "Parent process is now freed up since the child has been terminated!, see ya" << std::endl;
  }

  if (childProcess == 0) {
    std::string test;
    std::cout << "  In child process, go ahead and input your data:" << std::endl;
    std::cin >> test;
    std::cout << "  Thanks! Child quitting now :)" << std::endl;
  }

  return 0;
}
