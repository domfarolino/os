This folder contains code to create a trivial linux kernel module.

# Instructions

To build the module simply run `make`.

To dynamically insert the module into the kernel, run `sudo insmode dommod.ko`, and to remove this module run `sudo rmmod dommod`.
All messages from the kernel ring buffer can be printed with the `dmesg` command. This buffer is where the output of calls to `printk`
inside module code will end up. The `lsmod` command can be used to obtain a list of all inserted kernel modules.
