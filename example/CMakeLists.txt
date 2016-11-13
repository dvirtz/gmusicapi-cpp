set(SOURCES Example.cpp)

add_executable(Example ${SOURCES})

find_package(Boost CONFIG REQUIRED)

target_link_libraries(Example PRIVATE
	GMusicApi
	PythonHelper
	${PYTHON_LIBRARIES})