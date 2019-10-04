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
