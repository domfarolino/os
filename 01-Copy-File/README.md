This folder houses the C code to copy `input`'s contents to `output` which may or may not exist. If the `output` file does
exist, all of its contents will be overwritten with the copied file's contents, else a new one will be created with permissions
`-rw-r--r--`. To use this program run `make` and execute the resulting binary.

# System Calls

### `open`

The `open` system call in C is available via the file control options header file `<fcntrl.h>`, and returns the file descriptor
of an opened file as the smallest available integer greater than 0. Negative return value means there was an error opening the file.
The first argument is a `const char*` representing the name of the file to open. The second argument is a bit-or list of values that
determine how to open the file. The third argument is a bit-or list of values that set the permissions of the file descriptor if one
is created. There are 9 permission option settings, one for `r`, `w`, and `x` for the owner, group, and other users. They are as follows:

 - S\_IRUSR
 - S\_IWUSR
 - S\_IXUSR
 - S\_IRGRP
 - S\_IWGRP
 - S\_IXGRP
 - S\_IROTH
 - S\_IWOTH
 - S\_IXOTH

More info as well as a list of the enum values for methods to opening a file can be found in the man page for this system call
(see root README) or [here](http://pubs.opengroup.org/onlinepubs/9699919799/). The setting of permissions is interesting and prompted
me to finally "really" understand UNIX file permissions.

### `read`

The `read` system call is available to us via the `<unistd.h>` header file which is basically our program's window to the POSIX
API. This system call returns the number of bytes read or a negative integer indicating an error. The first argument is an `int`
file descriptor to read from. You can use something provided by `open()` or either 0, 1, or 2 to indicate stdin, stdout, or stderr
respectively. The second argument is a `const char*` buffer, and the third argument is the length of the buffer. This buffer will
be used to store some of the contents from a file.

### `write`

The `write` system call is available to us vai the `<unistd.h>` header. This system call returns the number of bytes written to the
output file descriptor, or a negative integer indicating an error. The first argument is an `int` file descriptor to write to. The
second is a `const char*` buffer to use as the source of content to write, and the third argument is the number of bytes to write from
the source buffer. `write` can be used in conjunction with `read` to copy a file's contents elsewhere. In this case the third argument
to `write` will be the value returned from `read` as we only want to write bytes from our buffer that we've previously read.
