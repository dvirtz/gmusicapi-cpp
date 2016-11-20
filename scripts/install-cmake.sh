#!/bin/sh
VERSION=$1

if [ ! -d /home/travis/cmake ]; then
	wget --no-check-certificate https://cmake.org/files/v$VERSION/cmake-$VERSION.0-Linux-x86_64.sh
	chmod +x cmake-$VERSION.0-Linux-x86_64.sh
	mkdir /home/travis/cmake
	./cmake-$VERSION.0-Linux-x86_64.sh --skip-license --prefix=/home/travis/cmake
fi
export PATH=/home/travis/cmake/bin:${PATH}