# Threads

This folder contains code related to thread creation and management in C.
Provided are examples of different programs using the `pthread` library.

This repository hosts both asynchronous threading examples (soon) and synchronous
threading examples. Asynchronous threading is where a parent thread creates
a child thread and resumes execution concurrently (and ideally in parallel)
to the child, and is most associated with task parallelism. Synchronous threading
is where a parent thread creates one or more children threads and synchronously
waits for them to complete, and is most associated with data parallelism.

# POSIX Threads (Pthreads)

Pthreads is a specification for thread creation and management whose
implementations appear on many Unix/Linux systems. It can be offered
as a library in either user or kernel space. It is this library that all
of the threading examples in this repository use until/unless support for
native Windows threads is added at some point :)

Below is a basic set of steps commonly performed when using the pthreads library
to create and manage threads.

 - First, setup the identifiers (`pthread_t`) and attributes (`pthread_attr_t`) for the threads
   we'll be spinning up.
 - Next, we initialize the thread attributes we've created (we'll mostly be using the defaults)
 - Then, we create the thread by giving it a function to run and the arguments for said function
 - Finally, we can wait for the thread to finish execution and join the parent thread so we can
   use its results!

The last two steps above breeze over some important things that pthreads complicates a bit. Two
things we often want to do when working with threads is:

 - Give a thread some data (or at least some direction on how to find data)
 - Use a thread's return value, or the results of its labor

Both of these involve sharing data between two threads of the same process, which is important to
understand. Pthreads complicates this just a bit because the function a thread begins execution in

 - Must take a single `void*` argument, and
 - Must also return a `void*`

Therefore, giving a thread multiple arguments is often done by passing in a struct to package the
arguments up. The struct is then cast to a `void*` and then back to a struct inside the threaded
function. Sexy huh? We're not limited by parameter passing to give a thread some data however. Any
global variables in a process can be accessed by the threads of that process. Of course local variables
defined inside of a function executing in a thread are available only to that thread.

Now that we've talked about giving a thread some data, let's figure out how to get data back to its
parent! It might be common to think that the best way to join the results of a thread is to return
some value or data from the function the thread began execution in. This doesn't work so well though,
because as we mentioned, pthreads requires our final return to be of type `void*`, and since access
to data declared on a thread's stack after execution (when it returns) is undefined since it no longer
exists, we can't exactly return local result variables as we'd like. Instead we can use globals that
get updated throughout a thread's execution (stay tuned for thread synchronization!), or make room in
our argument struct for a return value that we can modify in the thread and view in its parent. Alternatively
we can allocate memory on the heap and return a pointer to it, however the thread's caller is then
responsible for freeing the memory.

## Library functions/system calls

In this section are the library functions that correspond with some of the aforementioned
operations.

### `pthread_attr_init(pthread_attr_t *attr)`

This function takes a pointer to a `pthread_attr_t` object which it initializes with default
thread attributes. It requires a pointer because the contents of the variable (possibly old
attributes) passed in will be modified or initialized.

### `pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*func)(void *), void *arg)`

This function takes a thread identifier, thread attributes (or `NULL` to use the defaults), a function
to start the thread's execution in (must return a `void*`), and a single argument for the function
(must be a `void*`). As stated, the function a thread begins execution in must return a `void*` and
take a single `void*` argument. While this sounds less than ideal, structs are often used as the thread
argument when multiple arguments are needed.

The thread identifier will contain the value of the created thread's identifier when the function returns.

### `pthread_exit(void *value_ptr)`

This function takes in a `void*`, which it makes available to any thread joining with this newly-exited
thread (we'll see how to do this soon!). Think of `pthread_exit` as "returning" from a thread, and stopping
execution. A thread starts inside a function, and from there can call a chain of many other functions if
it so chooses. Each function may return values back to their callers as normal, however the function a
thread was created can either call `pthread_exit` with a `void*`, or return a `void*`. Returning implicitly
calls `pthread_exit` for you, so there's no need to invoke this function explicitly.

### `pthread_join(pthread_t thread, void **value_ptr)`

This function takes a thread identifier, and suspends execution of the current thread until the target thread
terminates (unless it is already finished). The second parameter of type `void **value_ptr` is the address of
a pointer that gets modified to point to the return value of the exiting thread. The double pointer might be
a little confusing, but all it means is that we must pass the address of a `void*` variable into the function.
This is because if a function wishes to modify a pointer, it needs a pointer (or a reference in C++) to the
pointer in question, instead of just a copy we mutate and throw away after the function executes. If this is
still confusing, consider the following basic example in which we make a function (analagous to `pthread_join`)
that modifies a pointer.

```c
// This function does absolutely nothing
void useless(int *dumPtr) { // we create a copy of the important pointer
  // We operate on the copy of the pointer to get what we want (cool!)
  dumPtr = &b;

  // Oh no, just as we return, the copy we have gets popped off the stack
  // and the originally passed in pointer remains untouched since we
  // worked on a copy of it :(
}

// Hey this function actually works
void changePtrToB(int **actualPtrNotACopy) {
  actualPtrNotACopy = &b;
}

int a = 10;
int* ptr = &a;
int b = 11;

// We can either type "ptr = &b", or call "changePtrToB(ptr)"
```

# Run the code!

Go ahead and run `make` in this directory to build some threaded programs!

In the first example we just take a simple function responsible for computing the sum of the
first `n` numbers and run it in a different thread. We give `n` to the thread as an argument,
and communicate the results of the function back to the main (parent) thread by means of a global
variable that we continually update. Once the thread joins back to the main (parent) thread, we
know that execution has halted and the global contains the final result, ready for the parent to
use and display. This simple program is trivial and doesn't really take advantage of a thread's
real power.

The second example is trivial as well, demonstrating the perils of passing a local variable to a
thread that goes out of scope when the thread is created.

In the third example, we demonstrate using a struct to package multiple arguments for a thread
together and provide a slot for the return value as well.

todo(domfarolino): fourth and so on (ideally thread-safe parallel examples demonstrating a thread's power)
