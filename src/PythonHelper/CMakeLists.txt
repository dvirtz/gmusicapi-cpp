set(SOURCES ModuleBase.cpp Initializer.cpp)
set(HEADERS 
	include/ModuleBase.h 
	include/ClassBase.h 
	include/utility.h 
	include/Initializer.h 
	include/typeConverters.h)

string(REPLACE "include/" "${INCLUDE_DIR}/PythonHelper/"
       HEADERS "${HEADERS}")

find_package(Boost CONFIG REQUIRED)

add_library(PythonHelper ${SOURCES} ${HEADERS})
	
target_include_directories(PythonHelper PUBLIC
	${INCLUDE_DIR})

target_include_directories(PythonHelper SYSTEM PUBLIC
	${PYBIND11_INCLUDE_DIR}
	${PYTHON_INCLUDE_DIRS})

target_link_libraries(PythonHelper PUBLIC
	Boost::boost
	${PYTHON_LIBRARIES})
