#include <iostream>
#include <cstdlib> // rand()
#include <thread>
#include <mutex>

// This is a really naive example where the ThreadableSeatBooker class is so
// simple, multithreading doesn't really improve our performance in the first
// place...but it's just a simple data race example.
class ThreadableSeatBooker {
public:
  ThreadableSeatBooker(int seat_limit) : seat_limit_(seat_limit),
                                         booked_seats_(0) {}

  void book_seat() { booked_seats_++; }

  void query() {
    std::cout << "\nBooked " << booked_seats_ << "/" << seat_limit_ <<
                 " seats" << std::endl;
  }

  // This is thread-safe!
  void book_seats() {
    while (booked_seats_ < seat_limit_) {
      // We can fit more seats, get the latest seat order.
      int num_seats_to_book = rand() % 13 + 1;

      // Comment this guard out to see the data race!
      std::lock_guard<std::mutex> lock(mu); // Unlocks at the end of the loop.
      if (booked_seats_ + num_seats_to_book <= seat_limit_) {
        std::cout << "Booking " << num_seats_to_book << ", ";
        booked_seats_ += num_seats_to_book;
      } else break;
    }
  }

private:
  int booked_seats_;
  int seat_limit_;
  std::mutex mu;
};

int main() {
  srand(time(NULL));
  ThreadableSeatBooker booker(300);
  std::thread booker_2(&ThreadableSeatBooker::book_seats, &booker);
  booker.book_seats();
  booker_2.join();
  booker.query();
  return 0;
}
