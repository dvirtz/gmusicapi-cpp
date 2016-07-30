set(SOURCES Example.cpp)

add_executable(Example ${SOURCES})

find_package(Boost REQUIRED COMPONENTS ${BOOST_PYTHON})

target_include_directories(Example PUBLIC
	$<TARGET_PROPERTY:GMusicApi,INTERFACE_INCLUDE_DIRECTORIES>)

target_link_libraries(Example PUBLIC
	GMusicApi
	PythonHelper
	${Boost_LIBRARIES}
	${PYTHON_LIBRARY_RELEASE})