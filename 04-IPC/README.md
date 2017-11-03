This folder contains code relating to interprocess communication techniques
implemented in C. Provided are examples of both shared memory and message passing
techniques. Below are two very basic and watered down descriptions of these two
techniques we can use for IPC. The descriptions are very high level and as such are
fairly incomplete. Feel free to submit a pull request if you feel something should
be cleared up or added.

# Shared memory

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
synchronizing the producer and the consumer so that the consumer doesn't try and consume data
that does not exist yet, and so the producer does not overwrite data that has not been consumed
yet.

# System calls

There are basically three steps to perform to create and utilize a shared memory object
for IPC.

 - First we create a shared memory object that the OS keeps track of and associates with
   a given name, and associate this shared memory object with a file descriptor. This is
   done via the `shm_open` system call, which takes in a unique name.
 - Next we configure its size with `ftruncate`.
 - Finally we map the region of shared memory to our process's address space which gives
   us a `void*` in return. This acts as our process's window to the shared memory.

### `shm_open`

The `shm_open` system call is used to create a shared memory object and a file descriptor
that it associated to the shared memory object. It's arguments are basically the same as
that of the `open` system call's, so not much new here. It makes sense for the producer to
attempt to create a new shared memory object for it and other processes to use, so we'll
see the `O_CREAT` flag involved in this call.

### `ftruncate`

The `ftruncate` system call is relatively simple, as it takes in a file descriptor as its first
argument, and a size as its second. It either truncates or extends the file referenced by the
file descriptor to the given length. It returns 0 upon success, and -1 upon failure.

### `mmap`

The mmap system call is a little more complex and very versatile. Basically it can map a portion of
a process's address space to the data represented by a given file descriptor. It returns the address
of said mapping (`void *`), or -1 upon failure. So we use it to create a place in a process's memory
for our shared memory object, and then utilize its return value (the actual address in memory) to read
and write from the file stored in our process's address space. The specifics of the arguments are complex
so instead of covering tons of variations, I'll just explain the ones we're using in this example.

`mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0)`

The first argument is the address inside our process's address space we'd like to start our mapping.
If we pass in 0 (and we don't specify MAP\_FIXED in the flags section) we're allowing the system to
choose the address for us in some implementation dependent way. If we specify MAP\_FIXED in the flags,
we're telling the system that it must start the mapping at the given address, or fail if it cannot.

The second argument is simply the size of the mapping.

The third argument is an OR list of protections for the memory region in our process's address space.
We can choose to disallow the pages from being accessed in any way, allow the pages to be read, written,
or executed.

The fourth argument is a flags arugment which specifies the type of mapped object and miscellaneous options.
Either `MAP_SHARED` or `MAP_PRIVATE` must be specified (claim the man page).

The fifth argument is the source file descriptor that we're allocating space in memory for.

The sixth argument specifies an offset. Yay.

## Run the code!

Go ahead and run `make` in this directory to build two examples of shared memory operations. The first
example (`01-*`) demonstrates a process (`01-shm-open`) creating a shared memory object in the kernel,
associating it with a unique name, and getting a file descriptor representation of it, all via the
`shm_open` system call. Finally, another process (`01-shm-unlink`) comes along and disassociates this
shared memory object in the kernel from its name (more or less reverting the work the first process did
and basically "cleaning up" after it). It may make sense for a consumer to be responsible for these kinds
of actions.

In the second example (`02-*`) we see an incredibly simple IPC example using shared memory, and the above
mentioned system calls. After reading the above descriptions and some of the man pages, what's happening
should be fairly obvious, but basically we're creating shared memory in one process, mapping it to the
address space of said process, writing to it, and doing the same in another process but instead of writing,
we're reading.

In the third example (`03-mmap-file`) we map an existing file (or open one if it doesn't exist) to main
memory, and then continue writing a message to the memory mapped to the process's address space to update
the actual file's contents. We `mmap` with the `MAP_SHARED` flag indicating that updates to the mapped memory
should persist in the original file, however we could use `MAP_PRIVATE` which would make a private copy of
the data in the file and map this to memory (copy-on-write).

The fourth is a basic example of a unix ordinary pipe. We use a pipe (basically a pair of file descriptors)
to write data from a parent to a child process. Ordinary pipes only share data between multiple processes
if they are somehow related (i.e., parent-child relationship).

----

# Message passing

Another way two processes can communicate is via message passing, which provides a
mechanism for IPC and the synchronization of actions without a shared memory space.
Processes can communicate directly, by explicitly referencing each other, or indirectly
by passing messages to a mailbox or port (similar to shared memory).

## System calls

### `coming_soon...`
