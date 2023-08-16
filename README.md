# execution_time_of_file_existence_check

This is used to measure the execution time of the process of checking for the existence of a file.

There are three file checking processes, each using the following functions.

- access
- stat
- fopen

## Usage

```text
Usage: ./a.out [FILE] [LOOP_COUNT]
```

## Example

```console
gcc main.c
./a.out dir
```

## Result

CPU : AMD Ryzen 5 5600X 6-Core Processor

```console
$ uname --kernel-name --kernel-release --kernel-version --machine --operating-system --hardware-platform --processor
Linux 5.15.90.1-microsoft-standard-WSL2 #1 SMP Fri Jan 27 02:56:13 UTC 2023 x86_64 x86_64 x86_64 GNU/Linux
$ lsb_release -a
No LSB modules are available.
Distributor ID: Ubuntu
Description:    Ubuntu 22.04.2 LTS
Release:        22.04
Codename:       jammy
$ mkdir dir
$ cd dir
$ for i in {1..100000}; do dd if=/dev/zero of=file$i bs=1024 count=1; done
$ cd ..
$ sudo sh -c "echo 3 >'/proc/sys/vm/drop_caches' && swapoff -a && swapon -a"
$ gcc main.c
$ ./a.out dir
stat:4.8385e-07
access:5.5645e-07
fopen:9.8323e-07
```
