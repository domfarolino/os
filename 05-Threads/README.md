# Threading & Concurrency

This folder consists of many threading and concurreny examples in C++11. The
goal is to implement and explain classic multithreading problems and their
solutions.

# 01-thread-intro

This is just a very simple example demonstrating the C++ std::thread API. It is
not very significant on its own. It demonstrates the following:

 - Running a function that takes a parameter off-main-thread
 - Running a _lambda_ function that takes a parameter off-main-thread
 - Calling an object member function from a different thread

# 02-data-races

This is an example showing how a class can be written to be thread-safe. We have
a class that contains a `seat_limit_`, and a number of `booked_seats_`. The
class provides a member function that pretends to query the latest seat order,
and if we can book the order without going over our limit, we do so by updating
`booked_seats_`.

When two threads operate on the same instance of this class, running the
`book_seats` method that continually fulfills seat orders creates a data race
between the threads, as the data member accesses are not synchronized. It is
possible for them to simultaneously update `booked_seats_`, after the comparison
deems it "safe" to do so, producing an over-booking.

We realize that the internal data members and their accesses are not
thread-safe, so we give the class a per-instance `std::mutex` to lock resources
that must be synchronized between threads. This example demonstrates using a
`std::mutex`, or even a `std::lock_guard`. As the code comment above
`ThreadableSeatBooker` mentions, since this is a trivial example, multithreading
doesn't really even improve our performance in the first place because the
operations are so cheap, but the takeaway is clear.
