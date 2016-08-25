cmake_minimum_required(VERSION 3.1.0)

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
set(GM_U "" CACHE STRING "Google Music user name")
set(GM_P "" CACHE STRING "Google Music password")

option(USE_STATIC_BOOST "Whether to use static boost libraries" FALSE)
set(Boost_USE_STATIC_LIBS ${USE_STATIC_BOOST})
# disable boost auto-link
add_definitions(-DBOOST_ALL_NO_LIB)

find_package(PythonInterp ${PYTHON_VERSION} REQUIRED)
if (TRAVIS)
	find_path(PYTHON_INCLUDE_DIR 
		NAMES Python.h
		PATHS $ENV{VIRTUAL_ENV}/include
		PATH_SUFFIXES
		python${PYTHON_VERSION}mu
		python${PYTHON_VERSION}m
		python${PYTHON_VERSION}u
		python${PYTHON_VERSION}
	)
	set(PYTHON_INCLUDE_DIR ${PYTHON_INCLUDE_DIR} CACHE STRING "Python include dir")
	execute_process(COMMAND ldd ${PYTHON_EXECUTABLE} COMMAND grep python OUTPUT_VARIABLE python_libraries OUTPUT_STRIP_TRAILING_WHITESPACE)
	string(REGEX REPLACE ".* => (.*) .*" "\\1" python_libraries ${python_libraries})
	set(PYTHON_LIBRARY ${python_libraries} CACHE STRING "Python library path")
endif()
find_package(PythonLibs ${PYTHON_VERSION} REQUIRED)

# find boost and python
if(${PYTHON_VERSION_MAJOR} EQUAL 3)
    set(BOOST_PYTHON python3)
else()
    set(BOOST_PYTHON python)
endif()

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

if(USE_STATIC_BOOST)
	add_definitions(-DBOOST_PYTHON_STATIC_LIB)
endif()

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(example)
