cmake_minimum_required(VERSION 3.1.0)

project(GMusicApi)

# require C++11 support
if("$ENV{TRAVIS}" STREQUAL "true")
	set(TRAVIS ON)
	# for some reason, Travis-CI doesn't support CMAKE_CXX_STANDARD variable
	message(STATUS "on Travis-CI")
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++1y" COMPILER_SUPPORTS_CXX14)
	if(COMPILER_SUPPORTS_CXX14)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
	else()
			message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
	endif()
else()
	message(STATUS "not on Travis-CI")
	set(CMAKE_CXX_STANDARD 14)
	set(TRAVIS OFF)
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
	find_program(PYTHON_CONFIG_EXECUTABLE python-config)
	if (NOT PYTHON_CONFIG_EXECUTABLE)
	    message(SEND_ERROR "python-config executable not found, but python is required.")
	endif()
	# using "python-config --prefix" so that cmake always uses the python that is
	# in the user's path, this is a fix for homebrew on Mac:
	# https://github.com/Homebrew/homebrew/issues/25118 [^]
	execute_process(COMMAND ${PYTHON_CONFIG_EXECUTABLE} --prefix OUTPUT_VARIABLE python_prefix OUTPUT_STRIP_TRAILING_WHITESPACE)
	set(PYTHON_INCLUDE_DIR ${python_prefix}/include/python2.7 CACHE STRING "Python include dir")
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
