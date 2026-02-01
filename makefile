# Generated Makefile, Just a template. You can modify me

Var0 = clang++

.DEFAULT_GOAL := all
clean:
	@echo "[INFO] Cleaning..."
	-@rm *.o
run:
	
src/files.cc.o: src/files.cc
	ccache ${Var0} -c src/files.cc -o files.cc.o  --std=gnu++23 -I includes
	@echo "[[[1;32m Compiling file src/files.cc 0.0%...[0m]]"
src/make.cc.o: src/make.cc
	ccache ${Var0} -c src/make.cc -o make.cc.o  --std=gnu++23 -I includes
	@echo "[[[1;32m Compiling file src/make.cc 20.0%...[0m]]"
src/inputfile.cc.o: src/inputfile.cc
	ccache ${Var0} -c src/inputfile.cc -o inputfile.cc.o  --std=gnu++23 -I includes
	@echo "[[[1;32m Compiling file src/inputfile.cc 40.0%...[0m]]"
src/sync.cc.o: src/sync.cc
	ccache ${Var0} -c src/sync.cc -o sync.cc.o  --std=gnu++23 -I includes
	@echo "[[[1;32m Compiling file src/sync.cc 60.0%...[0m]]"
src/main.cc.o: src/main.cc
	ccache ${Var0} -c src/main.cc -o main.cc.o  --std=gnu++23 -I includes
	@echo "[[[1;32m Compiling file src/main.cc 80.0%...[0m]]"
Link: src/files.cc.o src/make.cc.o src/inputfile.cc.o src/sync.cc.o src/main.cc.o
	clang++ files.cc.o make.cc.o inputfile.cc.o sync.cc.o main.cc.o -o c++make
	@echo "[[[1;32m Linking 100%...[0m]]"

all:	clean	run	src/files.cc.o	src/make.cc.o	src/inputfile.cc.o	src/sync.cc.o	src/main.cc.o	Link
install:
	sudo mv c++make /usr/bin