set(GMUSICAPI_PATH_H gmusicapiPath.h)
file(TO_NATIVE_PATH ${CMAKE_SOURCE_DIR}/gmusicapi GMUSICAPI_PATH)
string(REPLACE "\\" "\\\\" GMUSICAPI_PATH ${GMUSICAPI_PATH})
configure_file("${GMUSICAPI_PATH_H}.in" ${CMAKE_CURRENT_BINARY_DIR}/${GMUSICAPI_PATH_H})

set(HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${GMUSICAPI_PATH_H})

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(INCLUDE_HEADERS Module.h ClientBase.h Mobileclient.h Webclient.h Song.h 
	GeneratedRange.h AlbumArt.h SongQuality.h RegisteredDevice.h 
	genDefs.h Playlist.h RadioStation.h Album.h Artist.h SearchResults.h
	Genre.h Musicmanager.h Clients.h SharedPlaylistInfo.h)
foreach(header ${INCLUDE_HEADERS})
	list(REMOVE_ITEM INCLUDE_HEADERS ${header})
	list(APPEND INCLUDE_HEADERS ${INCLUDE_DIR}/gmusicapi/${header})
endforeach()

set(SOURCES Module.cpp Mobileclient.cpp Webclient.cpp ClientBase.cpp Musicmanager.cpp Clients.cpp)

add_library(GMusicApi ${SOURCES} ${HEADERS} ${INCLUDE_HEADERS})

target_include_directories(GMusicApi PUBLIC
	${INCLUDE_DIR}
	$<TARGET_PROPERTY:PythonHelper,INTERFACE_INCLUDE_DIRECTORIES>
	PRIVATE
	${CMAKE_CURRENT_BINARY_DIR})

target_compile_definitions(GMusicApi PUBLIC
	$<TARGET_PROPERTY:PythonHelper,INTERFACE_COMPILE_DEFINITIONS>)

target_include_directories(GMusicApi SYSTEM PUBLIC
	$<TARGET_PROPERTY:PythonHelper,INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>)

if(MSVC)
	set_source_files_properties(Module.cpp PROPERTIES COMPILE_FLAGS /wd4172)
	target_compile_options(GMusicApi PUBLIC /Wv:18)
endif()