REM check to see if boost folder is empty
echo Downloading https://sourceforge.net/projects/boost/files/boost/%BOOST_DOT_VERSION%/boost_%BOOST_VERSION%.7z
appveyor DownloadFile https://sourceforge.net/projects/boost/files/boost/%BOOST_DOT_VERSION%/boost_%BOOST_VERSION%.7z
mkdir %TEMP%\boost
7z e boost_%BOOST_VERSION%.7z %TEMP%\boost
pushd %TEMP%\boost
./bootstrap.sh --prefix=%BOOST_ROOT%
./b2 -j2 link=shared variant=release toolset=msvc --with-date_time --with-python install
popd