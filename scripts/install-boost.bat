REM check to see if boost folder is empty
appveyor DownloadFile https://sourceforge.net/projects/boost/files/boost/%BOOST_DOT_VERSION%/boost_%BOOST_VERSION%.7z
7z e %TEMP%\boost
pushd %TEMP%\boost
./bootstrap.sh --prefix=%BOOST_ROOT%
./b2 -j2 link=shared variant=release toolset=msvc --with-date_time --with-python install
popd