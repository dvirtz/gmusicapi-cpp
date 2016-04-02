set(SOURCES ModuleBase.cpp Initializer.cpp)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(INCLUDE_HEADERS ModuleBase.h ClassBase.h utility.h Initializer.h typeConverters.h)
foreach(header ${INCLUDE_HEADERS})
	list(REMOVE_ITEM INCLUDE_HEADERS ${header})
	list(APPEND INCLUDE_HEADERS ${INCLUDE_DIR}/PythonHelper/${header})
endforeach()

FIND_BOOST_PYTHON()

string(REGEX MATCH "[0-9]+" PYTHON_MAJOR ${PYTHONLIBS_VERSION_STRING})

add_library(PythonHelper ${SOURCES} ${INCLUDE_HEADERS})

target_compile_definitions(PythonHelper PUBLIC
	PYTHON_MAJOR=${PYTHON_MAJOR})
	
target_include_directories(PythonHelper PUBLIC
	${INCLUDE_DIR})

target_include_directories(PythonHelper SYSTEM PUBLIC
	${Boost_INCLUDE_DIR}
	${PYTHON_INCLUDE_DIRS})