# !/bin/sh

mkdir build
cd build

printf "Enable Argumentsea? (Y, N): "
read ARG
if [ $ARG == Y ]; then
    cmake -DARGUMENTSEA=ON ..
elif [ $ARG == y ]; then
    cmake -DARGUMENTSEA=ON ..
elif [ $ARG == N ]; then
    cmake -DARGUMENTSEA=OFF ..
elif [ $ARG == n ]; then
    cmake -DARGUMENTSEA=OFF ..
fi

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