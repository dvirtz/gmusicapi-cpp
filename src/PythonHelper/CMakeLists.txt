set(HEADERS ModuleBase.h ClassBase.h typeConverters.h utility.h ClassMember.h)
set(SOURCES)

FIND_BOOST_PYTHON()

add_library(PythonHelper ${HEADERS} ${SOURCES})
set_target_properties(PythonHelper PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(PythonHelper SYSTEM PUBLIC
	${Boost_INCLUDE_DIR}
	${PYTHON_INCLUDE_DIRS})