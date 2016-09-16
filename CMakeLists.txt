cmake_minimum_required(VERSION 3.1.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(HUNTER_STATUS_DEBUG ON)

include(HunterGate)

HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.15.25.tar.gz"
    SHA1 "74993ddf2cf8a6a4c3e0b9d58027a624bf3060e7"
	LOCAL
)

project(GMusicApi)

# require C++11 support
if("$ENV{TRAVIS}" STREQUAL "true")
	set(TRAVIS ON)
	set(CI ON)
	# for some reason, Travis-CI doesn't support CMAKE_CXX_STANDARD variable
	message(STATUS "on Travis-CI")
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++1y" COMPILER_SUPPORTS_CXX14)
	if(COMPILER_SUPPORTS_CXX14)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
	else()
		message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
	endif()
elseif("$ENV{APPVEYOR}" STREQUAL True)
	set(APPVEYOR ON)
	set(CI ON)
else()
	message(STATUS "not on CI")
	set(CMAKE_CXX_STANDARD 14)
	set(TRAVIS OFF)
	set(APPVEYOR OFF)
	set(CI OFF)
endif()

# Includes Catch in the project:
set(Catch_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Catch/include)

# default Cache values
set(BOOST_ROOT "" CACHE PATH "Boost location")
# disable boost auto-link
add_definitions(-DBOOST_ALL_NO_LIB)

find_package(PythonInterp REQUIRED)
#if (TRAVIS)
#	find_path(PYTHON_INCLUDE_DIR 
#		NAMES Python.h
#		PATHS $ENV{VIRTUAL_ENV}/include
#		PATH_SUFFIXES
#		python${PYTHON_VERSION}mu
#		python${PYTHON_VERSION}m
#		python${PYTHON_VERSION}u
#		python${PYTHON_VERSION}
#	)
#	set(PYTHON_INCLUDE_DIR ${PYTHON_INCLUDE_DIR} CACHE STRING "Python include dir")
#	execute_process(COMMAND ldd ${PYTHON_EXECUTABLE} COMMAND grep python OUTPUT_VARIABLE python_libraries OUTPUT_STRIP_TRAILING_WHITESPACE)
#	string(REGEX REPLACE ".* => (.*) .*" "\\1" python_libraries ${python_libraries})
#	set(PYTHON_LIBRARY ${python_libraries} CACHE STRING "Python library path")
#endif()
find_package(PythonLibsNew ${PYTHON_VERSION_STRING} REQUIRED)

hunter_add_package(Boost COMPONENTS python date_time program_options)

# the following resolves the issue in
# http://stackoverflow.com/questions/10660524/error-building-boost-1-49-0-with-gcc-4-7-0
if(MINGW)
	add_definitions(-D_hypot=hypot)
endif()

if(MSVC)
	add_definitions(/WX /W4)
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-Wall -Wpedantic -Werror)
endif()

include(doxygen.cmake)

enable_testing()

find_package(Boost CONFIG REQUIRED python)

add_definitions(-DBOOST_PYTHON_STATIC_LIB)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(example)
