# Raw Compiler (RC)

A small compiler for the raw programming language.
Takes .raw source and spits elf32 binary.

## Docs

Find them by:
```console
$ man rc
```
or
```console
$ rc --help
RC -- Compiles .raw files into elf32 binaries.
Usage: rc [opts] [source files]
to produce a binary. (more help on '$ man rc')
Options:
        --assemble, -c
                Just produce a .o file.
        --help, -h
                Print this message.
        --out, -o [path. default: prog]
                Set the binary's name.
        --compile, -S
                Just produce a .s file.
        --version, -v
                Tell the program's version.
```

## Dependencies

1. GCC \>v2.2
2. ANSI C standard library
3. GNU libc
4. UNIX standard library
5. Linux, Coreutils, POSIX 1998...
6. Make

## Build

```console
$ make
```
or
```console
$ cc src/*.c -o rc # if your gcc is very modern you might need some extra flags
```

## Install

```console
$ make clean install
```
