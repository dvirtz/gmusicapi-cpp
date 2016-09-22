set(TEST_FILES_H testFiles.h)
set(AUDIO_TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/audiotest_small.mp3)
set(IMAGE_TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/imagetest_10x10_check.png)
if(WIN32)
	string(REPLACE "/" "\\\\" AUDIO_TEST_PATH ${AUDIO_TEST_PATH})
	string(REPLACE "/" "\\\\" IMAGE_TEST_PATH ${IMAGE_TEST_PATH})
endif()
configure_file("${TEST_FILES_H}.in" ${TEST_FILES_H})

set(CONFIGURE_FILES ${TEST_FILES_H}.in)
source_group("Configure Files" FILES ${CONFIGURE_FILES})
set(HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${TEST_FILES_H} TestFixture.h)
set(SOURCES MobileclientTest.cpp TesterMain.cpp WebclientTester.cpp MusicmanagerTest.cpp ClientsTest.cpp TestFixture.cpp)

find_package(Boost CONFIG REQUIRED COMPONENTS python date_time program_options)

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
	Boost::python Boost::date_time Boost::program_options
	${PYTHON_LIBRARIES})

if(UNIX)
	get_filename_component(PYTHON_PATH ${PYTHON_LIBRARIES} DIRECTORY)
else()
	get_filename_component(PYTHON_PATH ${PYTHON_EXECUTABLE} DIRECTORY)
endif()

set(PATH_FOR_TEST ${PYTHON_PATH})

if(CI)
	set(TESTS "Mobileclient login")
else()
	set(TESTS "~[Webclient]")
endif()

add_test(NAME RunTests COMMAND Tester)

message(STATUS "Added test ${TEST_COMMAND}")

if(UNIX)
	set_tests_properties(RunTests PROPERTIES ENVIRONMENT "LD_LIBRARY_PATH=${PATH_FOR_TEST}")
else()
	set_tests_properties(RunTests PROPERTIES ENVIRONMENT "PATH=${PATH_FOR_TEST};PYTHONPATH=${PYTHON_PATH}/lib/site-packages")
endif()