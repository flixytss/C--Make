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
because the **code flow dosen't matter**, The File section is where you add the files to the Makefile. Like this  
**Everything must be in a newline**

#### File
``` bash
#: File
src/main.cc
src/main2.cc
src .cc
#: Link args
#: Compiling args
#: Out
#: Project
#: Include
#: Info
#: Run
#: Compilers filters
```
You can add a directory and it will add all the files inside the directory, You can also put a argument for get the file with the wanted extension

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
For everything you need a newline (Comments and statements), This will add arguments to the final link to get the executable

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
This will add the argument to the selected compiler, But you can use "All" as a compiler for use it for all the compilers

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
includes All
home/etc All
#: Info
#: Run
#: Compilers filters
```
To include directories, This is the same as the Compiler args. **You have to select the compiler**

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
includes All
home/etc All
#: Info
UseCcache
#: Run
#: Compilers filters
```
`#: Info` Is for putting extra configs like:
- UseCcache - If it is in, the Makefile will use ccache
- AddClean - This will add a "clean" function
- Linker - This is for add a personalized linker

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
includes All
home/etc All
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
includes All
home/etc All
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

- The **code flow does not matter**.  
- To make a comment, Use '#' in a newline.  
- And report for any *bug*