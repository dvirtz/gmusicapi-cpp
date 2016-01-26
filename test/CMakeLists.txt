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
set(TEST_FILES_H testFiles.h)
set(AUDIO_TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/audiotest_small.mp3)
set(IMAGE_TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/imagetest_10x10_check.png)
if(WIN32)
	string(REPLACE "/" "\\\\" AUDIO_TEST_PATH ${AUDIO_TEST_PATH})
	string(REPLACE "/" "\\\\" IMAGE_TEST_PATH ${IMAGE_TEST_PATH})
endif()
configure_file("${TEST_FILES_H}.in" ${TEST_FILES_H})

set(HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${USER_CRED_H} ${CMAKE_CURRENT_BINARY_DIR}/${TEST_FILES_H})
set(SOURCES MobileclientTest.cpp TesterMain.cpp WebclientTester.cpp MusicmanagerTest.cpp)

FIND_BOOST_PYTHON(date_time)

if(MSVC)
	set_source_files_properties(TesterMain.cpp PROPERTIES COMPILE_FLAGS /wd4702)
endif()

add_executable(Tester ${HEADERS} ${SOURCES} ${TEST_FILES_H}.in)

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
		COMMAND ${CMAKE_COMMAND} -E env "PATH=${PYTHON_PATH}" $<TARGET_FILE:Tester> "~[Webclient]")
endif()

add_test(NAME RunTests COMMAND Tester "~[Webclient]")

set_tests_properties(RunTests PROPERTIES ENVIRONMENT "PATH=${PYTHON_PATH};$ENV{PATH}")
