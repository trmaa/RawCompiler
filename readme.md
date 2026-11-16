# Raw Compiler (RC)

A small compiler for the raw programming language.
Takes .raw source and spits elf32 binary.

Raw is a systems programming language, but not like rust or c++: This is a
real systemps programming language.

Raw is lower level than C but still high level, abstracting from assembly
enough to have an AST and therefore being cross-platform (or rather,
corss architecture).

This compiler is ment to run on i386 hardware sins that's the kind of
assembly that it spits. But it can run on x86\_64.

I did this because I want a more modern/ergonomic language than pre ANSI C to
program in my 1998 computer.

See the examples to see the syntax (when i make them).

I'm making the AST and the parser still...

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

1. GCC (pre ansi is fine)
2. ANSI C standard library
3. GNU libc
4. UNIX standard library
5. Linux, Coreutils, POSIX 1998...
6. Make (optional)

## Build

```console
$ make
```
or
```console
$ cc src/*.c -o rc
# if your gcc is post ansi you will need some extra flags
# like -ansi, -Wno-implicit-int...
```

## Install

```console
$ make clean install
```
or
```console
$ cp rc /bin
$ cp rc.1 /usr/share/man/man1/
```
