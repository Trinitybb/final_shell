##Static Libraries • Dynamic Libraries • Runtime Plugin Loading • TEFEL Interfaces • Python Integration

This project implements a modular runtime-extensible Unix-style shell written in C.
It demonstrates:

- Static library (libshellcore.a)

- Dynamic shared libraries (libtext.so, libmath.so)

- Runtime plugin loading using dlopen / dlsym

- Dr. Duncan White–style TEFEL interface binding

- Makefile build automation

- Python execution through fork() + execvp()

##Project Architecture
myshell              (host shell executable)
 ├── libshellcore.a  (static library: built-ins, utilities)
 ├── libtext.so      (dynamic plugin: text commands)
 ├── libmath.so      (dynamic plugin: math commands)
 ├── cmdiface.c      (interface binder using dlopen/dlsym)
 └── lookup.c        (symbol resolver inspired by TEFEL)
##Plugins Included
Text Plugin (libtext.so)

Provides:

- words
- upper
- reverse
- vowels
- pigl atin

Math Plugin (libmath.so)

Provides:

- fact
- fib
- gcd
- isprime

##Build Instructions
Build everything:
make

Run:
export LD_LIBRARY_PATH=".:$LD_LIBRARY_PATH"
./myshell

##Demo Commands
help
load text
words "this is my final"
upper hello
reverse hello
vowels Trinity
load math
fact 5
fib 10
isprime 17
py hello
prompt "bsu> "
exit

##Author

Trinity A. Bowling
Department of Computer Science
Bowie State University
