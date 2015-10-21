set(HEADERS GMusicApi.h GMusicApiUser.h Mobileclient.h Webclient.h Song.h 
	typeConverters.h GeneratedRange.h AlbumArt.h SongQuality.h RegisteredDevice.h)
set(SOURCES GMusicApi.cpp Mobileclient.cpp Webclient.cpp GMusicApiUser.cpp)

FIND_BOOST_PYTHON()

add_library(GMusicApi ${SOURCES} ${HEADERS} ${INC_FILES})

if(MSVC)
	set_source_files_properties(GMusicApi.cpp PROPERTIES COMPILE_FLAGS /wd4172)
endif()

target_include_directories(GMusicApi SYSTEM PUBLIC
	${Boost_INCLUDE_DIR}
	${PYTHON_INCLUDE_DIRS})

target_include_directories(GMusicApi PUBLIC
	${CMAKE_SOURCE_DIR}/include)