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
./a.out /etc/passwd 1000000
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
$ gcc main.c
$ ./a.out /etc/passwd 1000000
stat:2.44573e-07
access:2.93829e-07
fopen:7.98551e-07
```
