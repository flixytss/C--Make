# !/bin/sh

mkdir build
cd build

read -p "Enable Argumentsea? (Y/N): " ARG
read -p "Enable C++23 (Y/N): " ARG1

ARGUMENTSEA=$([[ ${ARG,,} == y ]] && echo ON || echo OFF)
CPP23=$([[ ${ARG1,,} == y ]] && echo ON || echo OFF)

cmake -DARGUMENTSEA=$ARGUMENTSEA -DCPP23=$CPP23 ..

make
sudo mv c++make /usr/bin/c++make

SEA=/usr/lib/libargumentsea.so

if [ ! -f "$SEA" ]; then
    cd ..
    git clone https://github.com/flixytss/Argumentsea.git
    cd Argumentsea
    mkdir build
    make
    make install
    sudo mv include/argumentsea.hpp /usr/include
    cd ..
    rm -rdf Argumentsea
fi