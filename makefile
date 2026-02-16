# Generated Makefile, Just a template. You can modify me

Var0 = clang++

.DEFAULT_GOAL := all
clean:
	@@echo "[INFO] Cleaning..."
	-@rm home/*.o
run:
	
src/main.cc.o: src/main.cc
	@echo -e "[\e[1;32m Compiling file src/main.cc 0.0%... \e[0m]"
	@ccache ${Var0} -c src/main.cc -o home/main.cc.o --std=gnu++23 -I includes -I/home/daniel/.local/state/c++make/libraries/raylib/include
src/files.cc.o: src/files.cc
	@echo -e "[\e[1;32m Compiling file src/files.cc 20.0%... \e[0m]"
	@ccache ${Var0} -c src/files.cc -o home/files.cc.o --std=gnu++23 -I includes -I/home/daniel/.local/state/c++make/libraries/raylib/include
src/inputfile.cc.o: src/inputfile.cc
	@echo -e "[\e[1;32m Compiling file src/inputfile.cc 40.0%... \e[0m]"
	@ccache ${Var0} -c src/inputfile.cc -o home/inputfile.cc.o --std=gnu++23 -I includes -I/home/daniel/.local/state/c++make/libraries/raylib/include
src/make.cc.o: src/make.cc
	@echo -e "[\e[1;32m Compiling file src/make.cc 60.0%... \e[0m]"
	@ccache ${Var0} -c src/make.cc -o home/make.cc.o --std=gnu++23 -I includes -I/home/daniel/.local/state/c++make/libraries/raylib/include
src/sync.cc.o: src/sync.cc
	@echo -e "[\e[1;32m Compiling file src/sync.cc 80.0%... \e[0m]"
	@ccache ${Var0} -c src/sync.cc -o home/sync.cc.o --std=gnu++23 -I includes -I/home/daniel/.local/state/c++make/libraries/raylib/include
Link: src/main.cc.o src/files.cc.o src/inputfile.cc.o src/make.cc.o src/sync.cc.o
	@echo -e "[\e[1;32m Linking 100%... \e[0m]"

	@clang++ home/main.cc.o home/files.cc.o home/inputfile.cc.o home/make.cc.o home/sync.cc.o -o home/c++make -lraylibsit -L/home/daniel/.local/state/c++make/libraries/raylib/lib
all:	clean	run	src/main.cc.o	src/files.cc.o	src/inputfile.cc.o	src/make.cc.o	src/sync.cc.o	Link
install:
	sudo mv home/c++make /usr/bin