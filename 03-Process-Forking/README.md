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

Available in the `sys/wait.h` header, the `wait` and `waitpid` system calls allow a process to synchronously
wait for the termination or state change of a child process. The `wait` system call simply takes in a pointer
to an integer and waits for any child process to be terminated. Upon return it will fill its integer pointer
parameter with the status of the terminated child, and return the child's PID.

By default `waitpid` is a little more specific by allowing us to wait for some specific child process to terminate
by accepting the child-to-wait-for's PID as the first argument. If the first argument is `-1` the behavior is the same
as `wait` and it waits for *any* child process to terminate. The second argument is still a pointer to an integer, however
the third argument is an OR of up to two constants `WNOHANG` and `WUNTRACED` and allows us to listen to a little bit more than
just child termination.

`WNOHANG` is fairly interesting because it doesn't block on the termination of some process. Instead it only tries to reap the
exit status of already-terminated zombie processes, and if there isn't any, doesn't block on future termination, and just returns
`0`. For more information see this fabulous answer: https://stackoverflow.com/a/34845669/3947332.

`WUNTRACED` allows our parent process to see status information of stopped processes in addition to terminated processes.

The `sys/wait.h` header provides some useful macros we can use to get some information as to how a child process was
terminated or stopped. The most useful (IMO) and interesting are:

 - `WIFEXITED`
  - `WEXITSTATUS`
 - `WIFSIGNALED`
  - `WTERMSIG`
 - `WIFSTOPPED`
  - `WSTOPSIG`
