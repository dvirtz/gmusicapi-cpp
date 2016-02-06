set(HEADERS ModuleBase.h ClassBase.h typeConverters.h utility.h Initializer.h)
set(SOURCES ModuleBase.cpp Initializer.cpp)

FIND_BOOST_PYTHON()

add_library(PythonHelper ${HEADERS} ${SOURCES})

target_include_directories(PythonHelper SYSTEM PUBLIC
	${Boost_INCLUDE_DIR}
	${PYTHON_INCLUDE_DIRS})