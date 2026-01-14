# C--Make (C++Make but github changed it)

## Information about it
C++Make is a Makefile Generator. Made it with C++, Here is a usage example

## Usage
`$ C++Make make`
It will create a file named create.conf that is the base of our Makefile generator
`$ C++Make make`
Then it will use it for the generation
``` bash
#: File
#: Link args
#: Compiling args
#: Out
#: Project
#: Include
#: Info
#: Run
#: Compilers filters
```
The file will be like this

### Sections
The file is divided by sections: File, Info, Run, Etc... Normally the first section is the File section, But you can move the sections behind or in front of others sections
because the code flow dosen't matter, The File section is where you add the files to the Makefile. Like this

#### File
``` bash
#: File
src/main.cc
src/main2.cc
#: Link args
#: Compiling args
#: Out
#: Project
#: Include
#: Info
#: Run
#: Compilers filters
```
For each of the files you have to use a newline

#### Link args
``` bash
#: File
src/main.cc
src/main2.cc
src .cc
#: Link args
-lcurl
-lm
-lraylib
# Things like that
#: Compiling args
#: Out
#: Project
#: Include
#: Info
#: Run
#: Compilers filters
```
For everything you need a newline (Comments and statements), This will add arguments to the final link to get the executable, Also you can add a directory and it will add all the files inside the directory, You can also put a argument for get all the files inside with the wanted extension

#### Compiling args
``` bash
#: File
src/main.cc
src/main2.cc
src .cc
#: Link args
-lcurl
-lm
-lraylib
# Things like that
#: Compiling args
--std=c++23 clang++
#: Out
#: Project
#: Include
#: Info
#: Run
#: Compilers filters
```
This will add the argument to the selected compiler

#### Out
``` bash
#: File
src/main.cc
src/main2.cc
src .cc
#: Link args
-lcurl
-lm
-lraylib
# Things like that
#: Compiling args
--std=c++23 clang++
#: Out
home
#: Project
#: Include
#: Info
#: Run
#: Compilers filters
```
For putting the Out directory for all the files

#### Project
``` bash
#: File
src/main.cc
src/main2.cc
src .cc
#: Link args
-lcurl
-lm
-lraylib
# Things like that
#: Compiling args
--std=c++23 clang++
#: Out
home
#: Project
C++Make
#: Include
#: Info
#: Run
#: Compilers filters
```
For the executable name

#### Include
``` bash
#: File
src/main.cc
src/main2.cc
src .cc
#: Link args
-lcurl
-lm
-lraylib
# Things like that
#: Compiling args
--std=c++23 clang++
#: Out
home
#: Project
C++Make
#: Include
includes
home/etc
#: Info
#: Run
#: Compilers filters
```
To include directories

#### Info
``` bash
#: File
src/main.cc
src/main2.cc
src .cc
#: Link args
-lcurl
-lm
-lraylib
# Things like that
#: Compiling args
--std=c++23 clang++
#: Out
home
#: Project
C++Make
#: Include
includes
home/etc
#: Info
UseCcache
#: Run
#: Compilers filters
```
This is to set configurations (`UseCcache`), Now there is just one, Bust there is gonna be more

#### Run
``` bash
#: File
src/main.cc
src/main2.cc
src .cc
#: Link args
-lcurl
-lm
-lraylib
# Things like that
#: Compiling args
--std=c++23 clang++
#: Out
home
#: Project
C++Make
#: Include
includes
home/etc
#: Info
UseCcache
#: Run
@fastfetch
#: Compilers filters
```
Just to put command to execute before the compilation

#### Compilers filters
``` bash
#: File
src/main.cc
src/main2.cc
src .cc
#: Link args
-lcurl
-lm
-lraylib
# Things like that
#: Compiling args
--std=c++23 clang++
#: Out
home
#: Project
C++Make
#: Include
includes
home/etc
#: Info
UseCcache
#: Run
@fastfetch
#: Compilers filters
.c clang
.cc cc
.s nasm
.rust rustc
# etc...
```
Just to put a compiler to every file extension you are using

### Extra Info

The code flow does not matter.  
To make a comment, Use '#' in a newline.  
And report for any bug