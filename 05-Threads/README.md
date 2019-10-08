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

# 03-producer-consumer (condition variables)

This is an example implementation of the classic producer-consumer problem. It
demonstrates a producer thread producing messages for a consumer thread. The
threads are producing and consuming data at different rates, and need to
synchronize access to the message buffer they are reading/writing from/to.

## Producer

The producer side of things is pretty straight-forward. In this case, our
producer is pushing messages to a `std::queue`. However, since the consumer will
read and dequeue messages from the queue, and `std::queue` is not thread-safe by
default, we must wrap our accesses to this shared resource with a mutex, or
equivalently a `std::lock_guard`. Our example producer produces a random number
of messages, each with a random delay in between each.

## Consumer

Naturally the consumer wants to consume messages to the queue until it receives
some sort of "quit" message from the producer. (This is because an empty queue
does not mean that the producer is finished). The `std::queue` of messages is a
shared resource which is not thread-safe by default, so its accesses must be
guarded with some type a lock, just like in the producer's case. A
straightforward way to do this would be something like this:


```cpp
while (not received quit message) {
  1. acquire lock
  2. if (q is empty) {
    3a. continue;
  } else {
    // Can consume from queue.
    3a. consume q.front() data
    4. dequeue from q
  }
}
```

This works, however notice that it puts the consumer in a busy-waiting state
when there is no data to consume. When there is no data to consume, the consumer
thread is constantly spinning, acquiring and releasing the lock, polling the
queue. This is not an efficient approach for waiting for data. Instead, we'd
rather be "notified" when there is more data for us to consume. Luckily POSIX
specifies another synchronization primitive called a "condition variable", whose
implementation is available via `std::condition_variable`.

A `std::condition_variable` allows us to conditionally put a thread to sleep
until the condition variable is notified by another thread. We do this by
calling the condition variable's "wait" method.

### std::condition\_variable::wait(lock)

This method takes a `std::unique_lock` that must be locked. The method
immediately releases/unlocks the lock, and blocks the thread until the
variable's `notify_one` or a similar method is called.

### std::condition\_variable::wait(lock, predicate)

An unfortunate complication with POSIX condition variables is that a waiting
thread can spuriously awake, before a notificaton method is called. Therefore
when a waiting thread is awoken, you must re-check the condition that you wanted
to wait on in the first place, to check the validiity of the awakening. The
following pattern is pretty common.

```cpp
while (wait_condition) {
  condition_variable.wait(lock);
}
```

Fortunately, the STL provides a `wait` overload equivalent to the above code:

```cpp
condition_variable.wait(lock, fn);
```

The above code immediately invokes the `fn` function, which must return a
boolean, and have no arguments. Returning `true` indicates the thread can stop
waiting; false` puts you back in a waiting state. You can check
`wait\_condition` inside `fn`, just as you would in the explicit while loop.

<!-- Mutex status after spurious wake: https://stackoverflow.com/questions/41007503 -->
<details>
<summary><b>Mutex status after spurious wake</b></summary>
You may be wondering: if it is safe (thread\_safe) to check the
`wait\_condition` after a spurious wake-up? The answer is yes; `wait` unlocks
the mutex before sleeping, and a spurious wake-up can only happen when the mutex
is unlocked. On a spurious wake-up, the lock is reacquired, so you can guarantee
that `fn` and checking `wait\_condition` are always guarded by the lock, which
is often what you want. For more information see
https://stackoverflow.com/questions/41007503
</details>
