#include <iostream>
#include <cstdlib> // rand()
#include <vector>
#include <thread>
#include <mutex>
#include <stdexcept>
#include <condition_variable>

// Returns a random wait period in ms, weighted to return lower milliesconds
// more frequently.
int get_random_wait() {
  int random = rand() % 10 + 1; // [1, 10]
  if (random <= 4) random = 0; // [1, 4]
  else if (random <= 6) random = 20; // [5, 6]
  else if (random <= 8) random = 70; // [7, 8]
  else if (random <= 9) random = 100; // [9, 9]
  else if (random == 10) random = 200; // [10, 10]
  return random;
}

/**
 * ThreadableCircularBuffer
 * This class represents a thread-safe circular buffer. It is intended to be
 * accessed simultaneously by a producer thread and a consumer thread. The
 * |enqueue()| method should be called by producers, and will block if the
 * buffer is already full. The |dequeue()| method should be called by consumers,
 * and will block if the buffer is empty and there are no messages to consume.
 */
class ThreadableCircularBuffer {
public:
  ThreadableCircularBuffer(int buffer_size): buffer_(buffer_size), read_(0),
                                             write_(0), size_(0) {
    if (buffer_size < 1)
      throw std::logic_error("Buffer must have at least one empty position");
  }

  void enqueue(const std::string& message) {
    std::unique_lock<std::mutex> lock(mutex_);
    condition_.wait(lock, [&]() -> bool {
      bool can_skip_waiting = (size_ != buffer_.size());
        if (can_skip_waiting == false)
          std::cout << "\x1B[34m   Producer sleeping because buffer is full\x1B[00m" << std::endl;
      return can_skip_waiting;
    });

    buffer_[write_++] = message;
    write_ %= buffer_.size();

    size_++;
    lock.unlock();
    condition_.notify_one();
  }

  std::string dequeue() {
    std::unique_lock<std::mutex> lock(mutex_);
    condition_.wait(lock, [&]() -> bool {
      bool can_skip_waiting = (size_ > 0);
      if (can_skip_waiting == false)
        std::cout << "\x1B[34m   Consumer sleeping because buffer is empty\x1B[00m" << std::endl;
      return can_skip_waiting;
    });

    std::string message = buffer_[read_++];
    read_ %= buffer_.size();

    size_--;
    lock.unlock();
    condition_.notify_one();
    return message;
  }

private:
  std::mutex mutex_;
  std::condition_variable condition_;

  std::vector<std::string> buffer_;
  int read_, write_, size_; // Track next read- and write-position in |buffer_|.
};

// This program demonstrates the classic producer-consumer problem with threads
// synchronizing a shared circular buffer.

void producer(ThreadableCircularBuffer& buffer, int num_messages, std::mutex& stdout_mutex) {
  std::string message;
  for (int i = 1; i <= num_messages; ++i) {
    if (i == num_messages) message = "quit";
    else message = "Messages #" + std::to_string(i);

    buffer.enqueue(message);

    {
      std::unique_lock<std::mutex> lock(stdout_mutex);
      std::cout << "\x1B[33m Producer producing '" << message << "'\x1b[00m" << std::endl;
    }

    // Producer sleeps for a random amount of time.
    std::this_thread::sleep_for(std::chrono::milliseconds(get_random_wait()));
  }
}

void consumer(ThreadableCircularBuffer& buffer, std::mutex& stdout_mutex) {
  std::string message;
  while (message != "quit") {
    message = buffer.dequeue();

    {
      std::unique_lock<std::mutex> lock(stdout_mutex);
      std::cout << "\x1B[32m Consumer consuming '" << message << "'\x1B[00m" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(get_random_wait()));
  }
}

int main() {
  srand(time(NULL));
  std::mutex stdout_mutex; // For std::cout.

  int buffer_size = rand() % 16 + 1;
  int num_messages = rand() % 64 + 1;
  std::cout << "ThreadableCircularBuffer size: " << buffer_size << std::endl;
  std::cout << "Producer will make " << num_messages << " messages" << std::endl;

  ThreadableCircularBuffer threadable_buffer(buffer_size);
  std::thread producer_thread(producer, std::ref(threadable_buffer), num_messages, std::ref(stdout_mutex));
  consumer(threadable_buffer, stdout_mutex);
  producer_thread.join();
  return 0;
}
