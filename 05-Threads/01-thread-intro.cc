#include <iostream>
#include <thread>

// This function runs in another thread.
void async_fn() {
  std::cout << "Multithreading!" << std::endl;
}

int main() {
  std::cout << "Main thread starting up the new thread" << std::endl;
  std::thread thread_1(async_fn);
  thread_1.join();
  std::cout << "All finished" << std::endl;
  return 0;
}
