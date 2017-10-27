This folder contains code and documentation about unix processes, creating basic multiprocess
applications with the `fork()` system call in C, and waiting on child processes with the `wait`
and `waitpid` system calls.

# Processes

In C/C++ we get can the pid of our program's process with the `getpid()` function call which returns
a `pid_t` indicating calling process's ID. The `getppid()` function will return the calling process's
parent ID. Both of these functions are defined in the `unistd.h` header.

# System calls

### `fork`

When we want to create a new process we can invoked the `fork()` system call. In Unix-like operating systems
an invocation of this function makes an exact copy of the calling process's address space and gives it to a
child process. Both parent and child processes will begin executing at the next instruction after the call to
`fork`. Calls to `fork()` return a `pid` whose value is the 0 in the child process, the new process ID in the
parent process, and -1 in case of an error. It is useful to tell whether you're executing code in the context
of a parent or child process. For example, in Firefox's Gecko we see calls to
[`XRE_IsContentProcess`](http://searchfox.org/mozilla-central/source/dom/base/nsDOMNavigationTiming.cpp#300)
which is used to determine whether you're in the parent browser process or the child (think tab) content
process.

We can perform this check easily by testing the returned value of our call to fork, as its value will be
different in both the parent and child processes.

```cpp
int a = 10;
pid_t childProcessID = fork();

if (childProcessID == 0) {
  // Executing in the child process
  // a will be 10
} else {
  // Executing in the parent process
  // a will still be 10, remember it's just an address-space copy :)
}
```

### `wait`/`waitpid`

Available in the `sys/wait.h` header, the `wait` and `waitpid` system calls allow a process to wait for the
termination or state change of a child process. The `wait` system call simply takes in a pointer to an integer
and waits for any child process of the calling process to be terminated. When a child process terminates, it will
return its PID and fill the integer pointer with status information of the terminated child (more on this later).

By default, `waitpid` is a little more specific, allowing us to wait for some specific child process to terminate
by accepting the PID of the child-to-wait-for as its first argument. Alternatively, if the first argument is `-1`,
the behavior is the same as `wait`, and it waits for *any* child process to terminate. The second argument is still
a pointer to an integer, however the third argument is an OR of up to two constants `WNOHANG` and `WUNTRACED`, and
allows us to listen to a little bit more than just child *termination*.

`WNOHANG` is fairly interesting because it doesn't block on the termination of a process. Instead it only tries to
claim an already-terminated zombie process and return its PID, however if none exist it immediately returns `0` and
doesn't wait for termination. For more information, see this fabulous answer: https://stackoverflow.com/a/34845669/3947332.

`WUNTRACED` allows our parent process to wait for child processes to be *stopped* in addition to being terminated. This
can be important as if we're waiting on a process without the `WUNTRACED` option constant, and the process gets the stop
signal, the parent will never be notified because the child hasn't terminated, even though it is suspended. Therefore we
hit a bit of a deadlock in which neither child nor parent process is doing any work.

The `sys/wait.h` header provides some useful macros we can use to call on the integer we passed in to collect status
information on a terminated child process. Some of the more useful (IMO) and interesting are:

 - `WIFEXITED`
  - `WEXITSTATUS`
 - `WIFSIGNALED`
  - `WTERMSIG`
 - `WIFSTOPPED`
  - `WSTOPSIG`

# Examples

### `fork.c`

This basic demo creates four processes in total, demonstrates what it looks like for a process to wait
on its children to finish executing, and displays their PIDs in an intuitive formation in the terminal.

### `fork2.cpp`

This is a trivial demo.

### `forkChain.cpp`

This program is designed to take in an integer from stdin and create a completely linear chain of forked
child processes, such that no two child processes share the same parent. Naturally, multiple bare calls to
`fork()` increases the number of processes exponentially, however we can limit the number of processes
spawned by `fork()` by ensuring we only call `fork()` when we're in a child process. In this demo we
maintain an dynamically allocated array of `pid_t`s for our child processes. We want to loop through all
of the `pid_t`s and if we're in the child-most process, fork another one to our array. The way to check
to see if we're in the child-most process is to see if the value of `fork()` called for the previously
forked process was `0`. If it is, we can fork a new process to our array, and continue, where the next
process will check to see if the new value of `fork()` is `0`, so it can continue the chain. To kick
things off we fork the first process outside of the loop since there is no previous value of `fork()` to
check. From then on all the forking appears in the loop where we start by checking the value of `arr[0]`
to see if we can fork `arr[1]`. A catch is that we'll have to initialize all `pid_t`s in the array to
non-0 values so we don't always appear to be in the child process when we're in the intermediate parent
processes (the internal nodes in the process tree). We'll also want to `wait` on a child process if we
plan on displaying the tree in a reasonable way.

### `fork3.c`

This demo illustrates what it looks like to use the two available options `WNOHANG` and `WUNTRACED` as
the third parameter for the `waitpid`. In the parent we fork two child processes, one of which exits
immediately and one of which waits around to listen for a signal (as if it were doing work or something).
Later in the parent process we clean up the first child process which has become a zombie process since
it has exited, then shortly after begin waiting for our second child process to be terminated or stopped
via a signal. We then read its status information with the macros defined in `sys/wait.h`.

Question for the reader: What processes meet the criteria to fulfill a call to `waitpid` when we combine
the `WNOHANG` and `WUNTRACED` flags together in a single OR list?

Answer: The call to `waitpid(pid, &status, WNOHANG | WUNTRACED)` still returns immediately as expected, but
instead of looking only for already-terminated processes and returning immediately, it also looks for
already-stoppped processes and returns its findings (or 0). So as expected, we get the best of both worlds: an
immediately returning call to `waitpid`, but also the ability to pick up stopped processes instead of just zombie
processes, assuming these stopped processes are lready stopped by the time we look for them
(remember we're not waiting!).
