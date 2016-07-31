#!/bin/sh
set -e	
# check to see if boost folder is empty
if [ ! -d "$BOOST_ROOT/lib" ]; then
	wget https://sourceforge.net/projects/boost/files/boost/$BOOST_DOT_VERSION/boost_$BOOST_VERSION.tar.bz2 -O /tmp/boost_$BOOST_VERSION.tar.bz2;
	tar jxf /tmp/boost_$BOOST_VERSION.tar.bz2
	cd boost_$BOOST_VERSION
	./bootstrap.sh --prefix=$BOOST_ROOT
	./b2 -j2 link=shared variant=release cxxflags=-fPIC --with-date_time --with-python install
else
  echo 'Using cached boost.'
fi
