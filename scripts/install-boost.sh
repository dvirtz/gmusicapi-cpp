#!/bin/sh
set -e	
# check to see if boost folder is empty
if [ ! -d "$HOME/boost_1_60_0/lib" ]; then
	wget https://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.tar.bz2 -O /tmp/boost_1_60_0.tar.bz2;
	tar --bzip2 -xf /tmp/boost_1_60_0.tar.bz2;
	cd boost_1_60_0;
	./bootstrap.sh --prefix=$HOME/boost_1_60_0;
	./b2 install;
else
  echo 'Using cached boost.';
fi
