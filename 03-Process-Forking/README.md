This folder contains code and documentation about unix processes and creating basic multiprocess
applications with the `fork()` system call in C.

# Processes

In C/C++ we get can the pid of our program's process with the `getpid()` function call which returns
a `pid_t` indicating calling process's ID. The `getppid()` function will return the calling process's
parent ID.

# System calls

### `fork`

When we want to create a new process we can invoked the `fork()` system call. In Unix-like operating systems
an invocation to this function makes an exact copy of the calling process's address space and gives it to a
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
and waits for any child process to be terminated. When a child process terminates it will return its PID and fill
the integer pointer with the status of the terminated child.

By default, `waitpid` is a little more specific, allowing us to wait for some specific child process to terminate
by allowing us to pass in its PID as the first argument. If the first argument is `-1`, the behavior is the same
as `wait`, and it waits for *any* child process to terminate. The second argument is still a pointer to an integer,
however the third argument is an OR of up to two constants `WNOHANG` and `WUNTRACED`, and allows us to listen to a
little bit more than just child termination.

`WNOHANG` is fairly interesting because it doesn't block on the termination of a process. Instead it only tries to
claim the exit status of an already-terminated zombie process and return its PID, however if there is none it immediately
returns `0`, and doesn't wait for any child to terminate. For more information, see this fabulous answer:
https://stackoverflow.com/a/34845669/3947332.

`WUNTRACED` allows our parent process to see status information of stopped processes in addition to terminated processes.
This can be important as if we're waiting on a process without the `WUNTRACED` option constant, and the process gets the
stop signal, the parent will never be notified because the child hasn't terminated, even though it is suspended. Therefore
we hit a bit of a deadlock in which neither child nor parent process is doing any work.

The `sys/wait.h` header provides some useful macros we can use to get some information as to how a child process was
terminated or stopped. The most useful (IMO) and interesting are:

 - `WIFEXITED`
  - `WEXITSTATUS`
 - `WIFSIGNALED`
  - `WTERMSIG`
 - `WIFSTOPPED`
  - `WSTOPSIG`

# Examples

### `fork.c`

This basic demo creates four processes in total, demonstrates what it looks like for a process to wait
on its children to finish executing, and displays their PIDs in a pseudo-tree style in the terminal.

### `fork2.cpp`

This is a trivial demo.

### `fork3.c`

This demo illustrates what it looks like to use the two available options `WNOHANG` and `WUNTRACED` as
the third parameter for the `waitpid`. We clean up a zombie child process and then shortly after begin
waiting for our second child process to be terminated or stopped via a signal, and then read its status
information with the macros defined in `sys/wait.h`.

Question for the reader: What happens here when we combine the `WNOHANG` and `WUNTRACED` flags together
in a single OR list when we wait process termination?

Answer: The call to `waitpid(pid, &status, WNOHANG | WUNTRACED) still returns immediately as expected, but
instead of looking for ONLY terminated processes and returning immediately, it also looks for stoppped
processes and returns its findings (or 0) immediately. So as expected, we get the best of both worlds: an
immediately returning call to `waitpid`, but also the ability to pick up stopped processes, assuming they are
already stopped by the time we look for them (remember we're not waiting!).
