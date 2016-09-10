set(SOURCES Example.cpp)

add_executable(Example ${SOURCES})

find_package(Boost CONFIG REQUIRED COMPONENTS python)

target_link_libraries(Example PRIVATE
	GMusicApi
	PythonHelper
	Boost::python
	${PYTHON_LIBRARIES})