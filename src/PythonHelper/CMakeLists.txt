set(HEADERS ModuleBase.h ClassBase.h typeConverters.h utility.h Initializer.h)
set(SOURCES ModuleBase.cpp Initializer.cpp)

find_package(Boost REQUIRED COMPONENTS ${BOOST_PYTHON})

string(REGEX MATCH "[0-9]+" PYTHON_MAJOR ${PYTHONLIBS_VERSION_STRING})

add_library(PythonHelper ${HEADERS} ${SOURCES})

target_compile_definitions(PythonHelper PUBLIC
	PYTHON_MAJOR=${PYTHON_MAJOR})

target_include_directories(PythonHelper SYSTEM PUBLIC
	${Boost_INCLUDE_DIR}
	${PYTHON_INCLUDE_DIRS})