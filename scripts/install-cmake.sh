#!/bin/sh

if [ ! -d /home/travis/cmake ]; then
	wget --no-check-certificate https://cmake.org/files/v3.2/cmake-3.2.0-Linux-x86_64.sh
	chmod +x cmake-3.2.0-Linux-x86_64.sh
	mkdir /home/travis/cmake
	./cmake-3.2.0-Linux-x86_64.sh --skip-license --prefix=/home/travis/cmake
fi
export PATH=/home/travis/cmake/bin:${PATH}