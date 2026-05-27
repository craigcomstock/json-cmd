# libntech Example Project

[libntech](https://github.com/cfengine/libntech/) is a lightweight C library used in CFEngine.
This example project shows a minimal autotools setup, using libntech as a git submodule.

## Instructions

### Download

```
$ git clone https://github.com/cfengine/libntech_example.git
$ cd libntech_example
$ git submodule init
$ git submodule update
```

### Build dependency

```
$ cd libntech
$ git fetch --all --tags
$ ./autogen.sh
$ make -j32
```

### Build examples

```
$ ./autogen.sh
$ make -j32
```

## File size

Currently, the project has 2 versions of the same example - a program which prints `argv`.
One uses the `Seq` data structure from libntech (and allocation functions), the other does not use libntech at all.
After building with the steps above, you can compare the file size:

```
$ ls -al argv_printer*
-rwxr-xr-x  1 olehermanse  staff  10908 Jul 29 14:22 argv_printer_libs
-rwxr-xr-x  1 olehermanse  staff   9024 Jul 29 14:22 argv_printer_zero
```

(In this simple test, the library added 2K to file size).

# Authors

CFEngine was originally created by Mark Burgess with many contributions from around the world.
Thanks [everyone](https://github.com/cfengine/core/blob/master/AUTHORS)!

[CFEngine](https://cfengine.com) is sponsored by [Northern.tech AS](https://northern.tech)
