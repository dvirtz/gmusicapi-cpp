if ((NOT DEFINED GM_U) 
	OR (GM_U STREQUAL "")
	OR (NOT DEFINED GM_P)
	OR (GM_P STREQUAL ""))
	message(FATAL_ERROR "Please define Google Music credentials:
	GM_U:	user name
	GM_P:	password")
endif()

set(USER_CRED_H userCredentials.h)
configure_file("${USER_CRED_H}.in" ${USER_CRED_H})
set(AUDIOTEST_PATH_H audiotestPath.h)
set(AUDIOTEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/audiotest_small.mp3)
if(WIN32)
	string(REPLACE "/" "\\\\" AUDIOTEST_PATH ${AUDIOTEST_PATH})
endif()
configure_file("${AUDIOTEST_PATH_H}.in" ${AUDIOTEST_PATH_H})

set(HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${USER_CRED_H} ${CMAKE_CURRENT_BINARY_DIR}/${AUDIOTEST_PATH_H})
set(SOURCES MobileclientTest.cpp TesterMain.cpp WebclientTester.cpp MusicmanagerTest.cpp)

FIND_BOOST_PYTHON(date_time)

if(MSVC)
	set_source_files_properties(TesterMain.cpp PROPERTIES COMPILE_FLAGS /wd4702)
endif()

add_executable(Tester ${HEADERS} ${SOURCES})

target_include_directories(Tester PUBLIC ${Catch_INCLUDE_DIR} 
	$<TARGET_PROPERTY:GMusicApi,INTERFACE_INCLUDE_DIRECTORIES>)

target_link_libraries(Tester PUBLIC
	GMusicApi
	PythonHelper
	${Boost_LIBRARIES}
	${PYTHON_LIBRARIES})

find_package(PythonInterp REQUIRED)

get_filename_component(PYTHON_PATH ${PYTHON_EXECUTABLE} DIRECTORY)

if(NOT(TRAVIS))
	add_custom_command(TARGET Tester 
		POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E env "PATH=${PYTHON_PATH}" $<TARGET_FILE:Tester>)
endif()

add_test(NAME RunTests COMMAND Tester)

set_tests_properties(RunTests PROPERTIES ENVIRONMENT "PATH=${PYTHON_PATH};$ENV{PATH}")
