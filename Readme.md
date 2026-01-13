# C--Make (C++Make but github change it)

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
The file is divided by sections: File, Info, Run, Etc... Normally the first section is File, But you can move the sections behind or in front of others sections
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
for each of the files you have to use a newline