# Generated Makefile, Just a template. You can modify me

Var0 = clang++

Run:
	
src/main.cc.o:
	ccache ${Var0} -c src/main.cc -o home/main.cc.o --std=gnu++23 -I includes 
	@echo "[[1;32m Compiling file src/main.cc 0.0%...[0m]"
src/make.cc.o:
	ccache ${Var0} -c src/make.cc -o home/make.cc.o --std=gnu++23 -I includes 
	@echo "[[1;32m Compiling file src/make.cc 25.0%...[0m]"
src/inputfile.cc.o:
	ccache ${Var0} -c src/inputfile.cc -o home/inputfile.cc.o --std=gnu++23 -I includes 
	@echo "[[1;32m Compiling file src/inputfile.cc 50.0%...[0m]"
src/files.cc.o:
	ccache ${Var0} -c src/files.cc -o home/files.cc.o --std=gnu++23 -I includes 
	@echo "[[1;32m Compiling file src/files.cc 75.0%...[0m]"
Link:
	clang++ home/main.cc.o home/make.cc.o home/inputfile.cc.o home/files.cc.o -o home/C++Make
	@echo "[[1;32m Linking 100%...[0m]"

all:	Run	src/main.cc.o	src/make.cc.o	src/inputfile.cc.o	src/files.cc.o	Link
install:
	sudo mv home/C++Make /usr/bin