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

set(CONFIGURE_FILES ${USER_CRED_H}.in ${TEST_FILES_H}.in)
source_group("Configure Files" FILES ${CONFIGURE_FILES})
set(HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${USER_CRED_H} ${CMAKE_CURRENT_BINARY_DIR}/${TEST_FILES_H})
set(SOURCES MobileclientTest.cpp TesterMain.cpp WebclientTester.cpp MusicmanagerTest.cpp ClientsTest.cpp)

find_package(Boost REQUIRED COMPONENTS ${BOOST_PYTHON} date_time)

if(MSVC)
	set_source_files_properties(TesterMain.cpp PROPERTIES COMPILE_FLAGS /wd4702)
endif()

add_executable(Tester ${HEADERS} ${SOURCES} ${CONFIGURE_FILES})

target_include_directories(Tester PUBLIC ${Catch_INCLUDE_DIR} 
	$<TARGET_PROPERTY:GMusicApi,INTERFACE_INCLUDE_DIRECTORIES>
	PRIVATE
	${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Tester PUBLIC
	GMusicApi
	PythonHelper
	${Boost_LIBRARIES}
	${PYTHON_LIBRARY_RELEASE})

if(UNIX)
	get_filename_component(PYTHON_PATH ${PYTHON_LIBRARY_RELEASE} DIRECTORY)
else()
	get_filename_component(PYTHON_PATH ${PYTHON_EXECUTABLE} DIRECTORY)
endif()

set(PATH_FOR_TEST ${PYTHON_PATH})
if(NOT(${USE_STATIC_BOOST}))
	set(PATH_FOR_TEST "${PATH_FOR_TEST}\;${Boost_LIBRARY_DIRS}")
endif()

if(CI)
	add_test(NAME RunTests COMMAND Tester "Mobileclient login")
else()
	add_test(NAME RunTests COMMAND Tester "~[Webclient]")
endif()

if(UNIX)
	set_tests_properties(RunTests PROPERTIES ENVIRONMENT "LD_LIBRARY_PATH=${PATH_FOR_TEST}")
else()
	set_tests_properties(RunTests PROPERTIES ENVIRONMENT "PATH=${PATH_FOR_TEST}")
endif()

if(NOT(CI))
	if(UNIX)
		add_custom_command(TARGET Tester 
			POST_BUILD 
			COMMAND ${CMAKE_CTEST_COMMAND} -V)
	else()
		add_custom_command(TARGET Tester 
			POST_BUILD 
			COMMAND ${CMAKE_CTEST_COMMAND} -C ${CMAKE_CFG_INTDIR} -V)
	endif()
endif()
