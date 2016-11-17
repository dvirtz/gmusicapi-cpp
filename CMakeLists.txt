cmake_minimum_required(VERSION 3.1.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(HUNTER_STATUS_DEBUG OFF)

include(HunterGate)

HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.15.25.tar.gz"
    SHA1 "74993ddf2cf8a6a4c3e0b9d58027a624bf3060e7"
	LOCAL
)

project(GMusicApi)

# require C++14 support
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
set(Catch_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Catch/include)

hunter_add_package(Boost COMPONENTS date_time program_options)

if(MSVC)
	add_definitions(/WX /W4 /wd4172)
elseif(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-Wall -Wpedantic -Werror)
endif()

include(doxygen.cmake)

enable_testing()

find_package(Boost CONFIG REQUIRED)

add_subdirectory(pybind11)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(example)
