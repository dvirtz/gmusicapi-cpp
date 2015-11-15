#include "Module.h"
#include "RegisteredDevice.h"
#include "gmusicapiPath.h"
#include "Song.h"
#include "typeConverters.h"
#include "Playlist.h"
#include <iostream>

namespace GMusicApi
{

using namespace boost::python;

Module& Module::instance()
{
	static Module instance;
	return instance;
}

Module::Module()
{
	// must be called before dealing with python
	Py_Initialize();
    // need to be called before dealing with python's datetime
    PyDateTime_IMPORT;

	try
	{
		// add gmusicapi module to path
		auto sys = import("sys");
		list path = extract<list>(sys.attr("path"));
		path.append(gmusicapi_path);

		// import gmusicapi module
		auto module = import("gmusicapi");
		m_dict = module.attr("__dict__");
	}
	catch (const error_already_set&)
	{
		handlePythonException();
	}

	registerTypeConverters();
}


Module::~Module()
{
}

void Module::registerTypeConverters()
{
	// Python to C++ converters
	PySequenceToCppContainerConverter<SongRange>::registerConverter();
    PyGeneratorToGeneratedRangeConverter<Song>::registerConverter();
	PyToCppConverter<dict, Song>::registerConverter();
	PySequenceToCppContainerConverter<std::vector<std::string>>::registerConverter();
	PyToCppConverter<str, std::string>::registerConverter();
	PyToCppConverter<dict, AlbumArt>::registerConverter();
	PySequenceToCppContainerConverter<std::vector<AlbumArt>>::registerConverter();
	PySequenceToCppContainerConverter<std::vector<RegisteredDevice>>::registerConverter();
	PyToCppConverter<dict, RegisteredDevice>::registerConverter();
    PySequenceToCppContainerConverter<PlaylistRange>::registerConverter();
    PyGeneratorToGeneratedRangeConverter<Playlist>::registerConverter();

	// C++ to Python converters
	to_python_converter<nullptr_t, NullptrToNoneConverter>();
	to_python_converter<std::vector<Song>, CppContainerToPyListConverter<std::vector<Song>>>();
	to_python_converter<std::vector<AlbumArt>, CppContainerToPyListConverter<std::vector<AlbumArt>>>();
	to_python_converter<std::vector<std::string>, CppContainerToPyListConverter<std::vector<std::string>>>();
	to_python_converter<Song, StructToPyDictConverter<Song>>();
	to_python_converter<AlbumArt, StructToPyDictConverter<AlbumArt>>();
	to_python_converter<RegisteredDevice, StructToPyDictConverter<RegisteredDevice>>();
    to_python_converter<boost::posix_time::ptime, BoostPTimeToPyDateTimeConverter>();
}

} // namespace GMusicApi