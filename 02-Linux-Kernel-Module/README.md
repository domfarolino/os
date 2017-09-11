This folder contains code for a basic linux kernel module that utilizes the linux linked
list data structure.

All kernel modules seem to start out by `#include`ing `<linux/init.h`, `<linux/module.h>`, and `<linux/kernel.h>`. Then
it is up to you to define two functions, one that will be called when the module is dynamically inserted into the kernel
and one that will be called when it is removed. The names of these functions don't matter, as you'll be using the `module_init`
and `module_exit` macros to set the init and exit functions to the names of the custom functions you've defined. The init function
must return an integer, while the exit is void. There also exist some macros to set the metadata of a module such as author and licensing
information.

# Kernel functions

I learned that there are kernel specific functions for just about everything you can do in C in user-space. For example, to print
something to the kernel ring buffer, you wouldn't use `printf`, but instead the kernel equivalent `printk` which behaves the same
as `printf` but with a log level indicating the type of message. There also exist functions like `kmalloc` and `kfree` that behave
similarly to the `malloc` and `free` in user-space, but are specifically designed to manage kernel memory!

# Trivial kernel module format

```c
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

int customInit(void) {
  printk(KERN_INFO "This is an info message that will be output in the kernel ring buffer\n");
  return 0; // indicating no error upon initialization
}

void customExit(void) {
  printk(KERN_INFO "Peace out");
}

// Set the initialization and exit functions with these module macros
module_init(customInit);
module_exit(customExit);

// Set some metadata
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Same here bro");
MODULE_AUTHOR("Me obv");
```

# Instructions

To build the module simply run `make`. The Makefile for a linux kernel module is very important, so check it's contents out if you'd like.

To dynamically insert the module into the kernel, run `sudo insmod dommod.ko`, and to remove this module run `sudo rmmod dommod`.
The module makes use of the `printk` function which is the kernel equivalent of `printf`, whose output can be read by viewing the
kernel ring buffer with the `dmesg` command. The `lsmod` command can be used to obtain a list of all inserted kernel modules.
