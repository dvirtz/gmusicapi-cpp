#!/bin/sh
# git clone https://cmake.org/cmake.git cmake_clone
# cd cmake_clone
# ./bootstrap --system-curl
# make
# sudo make install
# cd ..
wget --no-check-certificate https://cmake.org/files/v3.2/cmake-3.2.0-Linux-x86_64.sh
chmod +x cmake-3.2.0-Linux-x86_64.sh
mkdir /home/travis/cmake
./cmake-3.2.0-Linux-x86_64.sh --skip-license --prefix=/home/travis/cmake
export PATH=/home/travis/cmake/bin:${PATH}