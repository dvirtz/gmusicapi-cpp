set(HEADERS 
	include/Module.h 
	include/ClientBase.h 
	include/Mobileclient.h 
	include/Webclient.h 
	include/Song.h 
	include/GeneratedRange.h 
	include/AlbumArt.h 
	include/SongQuality.h 
	include/RegisteredDevice.h 
	include/genDefs.h 
	include/Playlist.h 
	include/RadioStation.h 
	include/Album.h 
	include/Artist.h 
	include/SearchResults.h
	include/Genre.h 
	include/Musicmanager.h 
	include/Clients.h 
	include/SharedPlaylistInfo.h)

string(REPLACE "include/" "${INCLUDE_DIR}/gmusicapi/"
       HEADERS "${HEADERS}")

set(SOURCES Module.cpp Mobileclient.cpp Webclient.cpp ClientBase.cpp Musicmanager.cpp Clients.cpp)

add_library(GMusicApi ${SOURCES} ${HEADERS} ${HEADERS})

if(MSVC)
	set_source_files_properties(Module.cpp PROPERTIES COMPILE_FLAGS /wd4172)
	target_compile_options(GMusicApi PUBLIC /Wv:18)
endif()

target_link_libraries(GMusicApi PUBLIC PythonHelper)