cmake_minimum_required(VERSION 3.1.0)

project(GMusicApi)

# require C++11 support
if(DEFINED TRAVIS)
	# for some reason, Travis-CI doesn't support CMAKE_CXX_STANDARD variable
	message(STATUS "on Travis-CI")
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	if(COMPILER_SUPPORTS_CXX11)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	elseif(COMPILER_SUPPORTS_CXX0X)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	else()
			message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
	endif()
else()
	message(STATUS "not on Travis-CI")
	set(CMAKE_CXX_STANDARD 11)
endif()

# always include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

# Includes Catch in the project:
set(Catch_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Catch/include)

# default Cache values
set(BOOST_ROOT "" CACHE PATH "Boost location")
set(GM_U "" CACHE STRING "Google Music user name")
set(GM_P "" CACHE STRING "Google Music password")

set(Boost_USE_DEBUG_PYTHON ON)
set(Boost_USE_STATIC_LIBS ON)
# disable boost auto-link
add_definitions(-DBOOST_ALL_NO_LIB)

# find boost and python
macro(FIND_BOOST_PYTHON)
find_package(Boost REQUIRED COMPONENTS python)
find_package(PythonLibs REQUIRED)
add_definitions(-DBOOST_DEBUG_PYTHON -DBOOST_PYTHON_STATIC_LIB)
endmacro()

# the following resolves the issue in
# http://stackoverflow.com/questions/10660524/error-building-boost-1-49-0-with-gcc-4-7-0
if(MINGW)
    add_definitions(-D_hypot=hypot)
endif()

if(MSVC)
	add_definitions(/WX /W4)
endif()

include(doxygen.cmake)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
