# C--Make (C++Make but github changed it)

## Information about it
C++Make is a Makefile Generator. Made it with C++, Here is a usage example

## Requirements
- cmake
- make
- simdjson

## Usage (Conf)
`$ C++Make make`
It will create a file named create.conf that is the base of our Makefile generator  
`$ C++Make make c.conf` You can also add a file name, It will create the file if it dosen't exists
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

``` bash
#: Windows
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
#: Linux
```
`#: Compilers filters`: Just to put a compiler to every file extension you are using
`#: Include`:  To include directories, This is the same as the Compiler args. **You have to select the compiler**
`#: Run`: Just to put command to execute before the compilation
`#: Project`: For the executable name
`#: Out`: For putting the Out directory for all the files
`#: Compiling args`: This will add the argument to the selected compiler, But you can use "All" as a compiler for use it for all the compilers
`#: Link args`: This will add arguments to the final link to get the executable
`#: File`: You can add a directory and it will add all the files inside the directory, You can also put a argument for get the file with the wanted extension

#### OSs Section

Here you can add a specific operative system config, These are the avaible operative systems:
- Linux
- Windows
- Mac

`#: Info` Is for putting extra configs like:
- UseCcache - If it is in, the Makefile will use ccache
- AddClean - This will add a "clean" function
- Linker - This is for add a personalized linker
- OutputFile - Change the output filename
- Cores - To set parallel build, And set the build cores (Very useless)
- Use - To copy an OS section, You can modify it without modifying the selected OS section
- OnlyUseLinker - It will use the linker, It will not add compile functions. It will just link the files that you added
- UseLib - It let you use your locals libraries (**More information at the final of this readme**), you just have to add the local library name and the compiler it will set it, like this `UseLib raylib clang++`
- UseLauncher - To set a launcher to a selected compiler

In the first section called "Windows" Made config in front of it part of windows, You can add more OSs sections Like Mac and Linux  
**Depending on the os name, the config will be used by the specific os**. If there isn't a config for you os, you will get the last config

## Usage (Json) (NEW)

You can make a makefile with just a json file, Just like this:
```json
{
    "project-name": "C++m",
    "files": [
        ["src", ".cc"],
        "file1.cc"
    ],
    "buildir": "home",
    "compilers-filter": {
        ".cc": "clang++"
    },
    "run-before": ["@echo \"Causa-san\""],
    "link-args": ["-lsimdjson"],
    "compile-args": {
        "clang++": "--std=gnu++23"
    },
    "includes": {
        "clang++": "includes"
    },
    "extra": {
        "only-linker": false,
        "use-ccache": false,
        "add-clean": true,
        "use-linker": "clang++",
        "use-lib": ["raylib", "clang++"],
        "set-cores": 0,
        "use-launcher": ["ccache", "clang++"]
    }
}
```
Just read it, It is very easy to understand. First thing that we read is `project-name`: is just the project name
- `files` - Is the files that the makefile gonna has, You can put an array or a string with the file location, In the array, The first argument 
is where are the files that are going to be include in the makefile (**It can be null to add all the files inside the directory**), **And the second argument will see if the file ends with itself, if not, the file will not be include in the makefile**
- `buildir` - Set the output directory
- `compilers-filter` - It will set a compiler to the files that end with the first argument
- `run-before` - It set the commands that are going to be in the makefile before the compilation section
- `link-args` - Set the linker arguments
- `compile-args` - It will set the selected argument to the selected compiler
- `includes` - It will include a directory to the selected compiler
- `extra` - Are extras, Which are these:
    - `only-linker` - If enabled, The makefile is going to use the given files to link it all without the compiling section
    - `use-ccache` - If enabled, The makefile is going to use ccache
    - `add-clean` - It will add a clean function
    - `use-linker` - Set a different linker
    - `use-lib` - It will add a library installed in `~/.local/state/c++make/libraries/` to the selected compiler: `use-lib: ["raylib", "clang++"]`
    - `set-cores` - It will enable the parallel builds mode with the selected cores
    - `use-launcher` - It require an array of two items: The launcher and the compiler

## Executable Arguments

`$ C++Make make template` Will create a template with the following things:
- src - Directory: Sources
- build - Directory: Out directory for all the objects file and the executable
- include - Directory: Files for the headers files
- src/main.cc - File: Just a c++ file  

`$ C++Make help` Just the help command (Just says something stupid...)  
Now in c++make (**NEW**) you can compile an individual file with `$ C++Make <<C/C++ file>>` and it will compile it

### Sync mode

Sync mode detect if the config file has changed, If it does, Then it will create a new makefile  
`$ C++Make sync <<Config file>> &` It will sync with the config file, It will read it looking for changes to remake the makefile.
**It has to be used as a background proccess with `&`**

### Libraries mode (NEW)

Now in c++make you can have yours libraries in your own user, using the location `~/.local/state/c++make/libraries/`. There will be all of yours libraries  
¿How can I install a library locally? You just have to go for your folder with the next directorys:
- include
- lib (optional)
And then use the command `c++make install <<The folder>>` and it will extract all the executables, header files and libraries .so. Creating a directory in 
`~/.local/state/c++make/libraries/` with the your directory name as the name  
¿How can I delete a library? Just use `c++make install <<The library>>`. You can see your libraries with `c++make libraries`

### Argumentsea (NEW)

This is a new feature, when you are compiling the **c++make** with the new sh file, it will ask you if you want to include **argumentsea**.
¿What is argumentsea? It's a new library made for ME (i'm the better, i already knew that :3 (**syfm**)). It will let you manage your program arguments more easy to use

### Extra Info

- The **code flow does not matter**.  
- To make a comment, Use '#' in a newline.  
- And report for any *bug*  
  
The `C++MakeSignature!` at the start of the code are the magic bytes, is it used for identifying a C++Make file without knowing the file name