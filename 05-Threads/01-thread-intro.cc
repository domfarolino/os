#include <iostream>
#include <thread>

// This function runs in another thread.
void async_fn(int in_val) {
  std::cout << "Multithreading with value: " << in_val << std::endl;
}

class CrossThreadObj {
public:
  CrossThreadObj(int data) {
    data_ = data;
  }

  void get_data() {
    std::cout << "CrossThreadObj data is: " << data_  <<
                 " on thread: " << std::this_thread::get_id() << std::endl;
  }

private:
  int data_;
};

int main() {
  std::cout << "Main thread starting up the new thread" << std::endl;
  std::thread thread_1(async_fn, 25 /* value */);
  // Immediately join these threads so that std::out is guaranteed to be clean.
  // This is just a simple example where we're demonstrating the API and
  // capabilities.
  thread_1.join();

  int tmp_val = 100;
  std::thread thread_2([=](int in_val) {
    std::cout << "Multithreading in a lambda with value: " << in_val <<
                 " and tmp_val: " << tmp_val << std::endl;
  }, 26 /* in_val */);
  thread_2.join();

  CrossThreadObj cobj(200);
  cobj.get_data();
  std::thread thread_3(&CrossThreadObj::get_data, &cobj);
  thread_3.join();

  std::cout << "All finished" << std::endl;
  return 0;
}
