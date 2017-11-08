# IPC

This folder contains code relating to interprocess communication techniques
implemented in C. Provided are examples of

 - [POSIX shared memory](#posix-shared-memory)
 - [Ordinary pipes](#ordinary-pipes)
 - Named pipes (fifos)
 - System V message queues

Below are two very basic and watered down descriptions of these shared memory
and message passing used for IPC. The descriptions are very high level and as
such are fairly incomplete. Feel free to submit a pull request if you feel
something should be cleared up or added.

# POSIX shared memory

Two processes can communicate via a chunk of shared memory. One process can
create a chunk of shared memory which the OS associates with a file descriptor.
Sharing this region of memory with another process means both processes agree to
remove the restriction that the OS normally enforces to make sure one process has
absolutely no access to another process's address space. Shared memory is sometimes
faster than message passing because the only kernel intervention required is when we
initially setup the region of shared memory. After this, the communicating processes
are responsible for synchronizing themselves, and formatting the data they share.

A solution to the producer/consumer problem using shared memory involves using a buffer
in the shared memory and two pointers, one pointing to the next consumable item and the
other pointing to the next slot the producer can insert an item in. The important part is
synchronizing the producer and the consumer so that the consumer doesn't try and consume
data that does not exist yet, and the producer doesn't overwrite data that has not been
consumed yet.

## System calls

Using shared memory for IPC can be done with both the POSIX and System V APIs. In this example
we're using the POSIX API for shared memory, which utilizes files mapped to a process's address
space. There are basically three steps to perform to create and utilize a shared memory object
for IPC:

 - First, we create a shared memory object that the OS keeps track of and associates with
   a file descriptor. This is done via `shm_open`.
 - Next, we configure the size of the shared memory object with `ftruncate`.
 - Finally, we map the region of shared memory to our process's address space. This is done
   with `mmap` which gives us a `void*` in return and acts as our process's window to the
   shared memory.

### `shm_open(const char *name, int oflag, mode_t mode)`

The `shm_open` system call is used to create a shared memory object accessible via the returned
file descriptor associated with the shared memory object. It's arguments are basically the same
as that of the `open` system call's, so not much new here. It makes sense for a producer to attempt
to create a new shared memory object for it and other processes to use, so we'll often see the
`O_CREAT` (and possibly `O_EXCL`) `oflag`s with this call.

### `ftruncate(int fildes, off_t length)`

The `ftruncate` system call is relatively simple, as it takes in a file descriptor as its first
argument, and a proposed file size as its second. It either truncates or extends the file or shared
memory object referenced by the file descriptor to the given length.

### `mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)`

The mmap system call is a little more complex and very versatile. Since `read` and `write` behavior
is undefined when the input file descriptor refers to a shared memory object (POSIX), we can interact
with shared memory by mapping the shared memory object to a portion of our process's address space and
reading and writing to it. This system call returns the address of said mapping (`void *`), or -1 upon
failure. The specifics of the arguments are a tad complex, so instead of covering tons of variations,
I'll just explain what we're using in this example.

`mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0)`

The first argument is the address inside our process's address space we'd like to start our mapping.
If we pass in 0 (and don't specify MAP\_FIXED in the flags section) we're allowing the system to
choose the address for us in some implementation dependent way. If we pass in another address, the
system will try and create the mapping there if one does not already exist. If one exists, it will
choose a different place, or overwrite the mapping if `MAP_FIXED` is given as a flag.

The second argument is simply the size of the mapping.

The third argument is an OR list of protections for the memory region in our process's address space.
We can choose to disallow the pages from being accessed in any way, allow the pages to be read, written,
or executed. This should not conflict with the open mode of the file.

The fourth argument is a flags arugment which specifies the type of mapped object and miscellaneous options.
Either `MAP_SHARED` or `MAP_PRIVATE`. With `MAP_SHARED`, the mapping is shared with other processes that map
the same file. Note that updated contents may not be written to the underlying file before `msync` or `munmap`
is called. With `MAP_PRIVATE`, an copy-on-write mapping is created, private to the calling process.

The fifth argument is the source file descriptor that we're allocating space in memory for.

The sixth argument specifies an offset. Yay.

### `msync(void *addr, size_t length, int flags)`

This system call flushes changes from a certain memory range that was mapped via `mmap` back to disk.
Without this, there is no guarantee that the memory will be written back before `munmap`.

### `munmap(void *addr, size_t len);`

This deletes a mapping associated with a specific address range. Upon calling, changes made to a shared
mapping are flushed to the underlying file. Mappings are unmapped by default when a process terminates.

----

# Ordinary pipes

One way two processes can communicate with each other without using shared memory is by using an
ordinary (anonymous) pipe. The caveat with an ordinary pipe is that it is unidirectional and that
it can only be shared with related processes, most often in a parent-child relationship. The pipe
is really just a pair of file descriptors, one for reading, and one for writing.

## System calls

### `pipe(int fildes[2])`

This system call takes in an array of two integers which it uses as file descriptors. The first
file descriptor corresponds with the read end of the pipe while the second corresponds with the
write end. We can create the array of file descriptors in a parent process and call `fork()` to
spawn a child process that has duplicated these descriptors. We can `write` to the write end of
the pipe from one process and `read` from the read end in another. It may feel bidirectional in
that it is possible to `write` to a child and then later `read` from the same child, but there
still only exists a single read/write end which makes it unidirectional. For more information
see https://stackoverflow.com/questions/8390799.

----

# Named pipes (fifos)

Named pipes, or fifos as they're often referred to as in Unix-like kernels, are a special type of
file that contains no contents in the filesystem, and whose existence merely acts as a way to reference
the pipe from other processes.

## System calls

### `mkfifo(const char *path, mode_t mode)`

This system call takes in a path which acts as a unique identifier for the pipe/fifo, and a permission
mode, and returns 0 on success, and -1 on failure. This function does not provide a file descriptor, it
just creates a special type of file whose type is "named pipe (fifo)". We can proceed to use `open` to
get the file descriptor, and call `read` and `write` as appropriate to give and get data to and from other
processes that open the pipe. Since the file contains no contents, data isn't stored in the filesystem so
a call to `open` with write capabilities hangs until a reader comes along with read capabilities for data
to stream to.

### `unlink(const char *path)`

This system call removes a link to the given path and decrements the link count of the file referenced by
path. If the link count is 0 and no processes have the file open, the resources are reclaimed by the kernel.

# Message passing

Another way two processes can communicate is via message passing, which provides a
mechanism for IPC without a shared memory space. Processes can communicate directly,
by explicitly referencing each other, or indirectly by passing messages to a mailbox
or port (similar to shared memory).

## System calls

### `coming_soon...`

# Run the code!

Go ahead and run `make` in this directory to build examples of programs that use IPC. The first
example (`01-*`) demonstrates a process (`01-shm-open`) creating a shared memory object, associating
it with a unique name, and getting a file descriptor representation of it via `shm_open`. Finally,
another process (`01-shm-unlink`) comes along and removes the shared memory object's name, and once
all processes unmap the data corresponding to the object, removes the object from the kernel.

In the second example (`02-*`) we see an incredibly simple shared memory example. After reading the
above descriptions and some of the man pages, what's happening should be fairly obvious, but basically
what we're doing is:

 - Creating a shared memory object in a producer process
 - Mapping it to the address space of said process
 - Writing to it
 - Doing the same in a consumer process but reading the data instead of writing

In the third example (`03-mmap-file`), we map an existing file (or open one if it doesn't exist) to memory,
and then continue writing a message to the mapped memory to update the actual file's contents. We `mmap` with
the `MAP_SHARED` flag indicating that updates to the mapped memory should persist in the original file, however
we could use `MAP_PRIVATE` which would make a private copy of the data (copy-on-write) whose changes will not be
reflected outside this process.

The fourth example is a basic demonstration of a Unix ordinary (anonymous) pipe. We use a pipe
(basically a pair of file descriptors) to write data from a parent to a child process.

<!-- Todo: move some of this section to an "Ordinary Pipe" # section -->
