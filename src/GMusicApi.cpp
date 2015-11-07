#include "GMusicApi.h"
#include "RegisteredDevice.h"
#include "gmusicapiPath.h"
#include "Song.h"
#include "typeConverters.h"
#include <iostream>

namespace GMusicApi
{

using namespace boost::python;

GMusicApi& GMusicApi::instance()
{
	static GMusicApi instance;
	return instance;
}

GMusicApi::GMusicApi()
{
	// must be called before dilling with python
	Py_Initialize();

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


GMusicApi::~GMusicApi()
{
}

void GMusicApi::registerTypeConverters()
{
	// Python to C++ converters
	PySequenceToCppContainerConverter<SongRange>::registerConverter();
	pyGeneratorToSongRangeConverter::registerConverter();
	PyToCppConverter<dict, Song>::registerConverter();
	PySequenceToCppContainerConverter<std::vector<std::string>>::registerConverter();
	PyToCppConverter<str, std::string>::registerConverter();
	PyToCppConverter<dict, AlbumArt>::registerConverter();
	PySequenceToCppContainerConverter<std::vector<AlbumArt>>::registerConverter();
	PySequenceToCppContainerConverter<std::vector<RegisteredDevice>>::registerConverter();
	PyToCppConverter<dict, RegisteredDevice>::registerConverter();

	// C++ to Python converters
	to_python_converter<nullptr_t, NullptrToNoneConverter>();
	to_python_converter<std::vector<Song>, CppContainerToPyListConverter<std::vector<Song>>>();
	to_python_converter<std::vector<AlbumArt>, CppContainerToPyListConverter<std::vector<AlbumArt>>>();
	to_python_converter<std::vector<std::string>, CppContainerToPyListConverter<std::vector<std::string>>>();
	to_python_converter<Song, StructToPyDictCoverter<Song>>();
	to_python_converter<AlbumArt, StructToPyDictCoverter<AlbumArt>>();
	to_python_converter<RegisteredDevice, StructToPyDictCoverter<RegisteredDevice>>();
}

} // namespace GMusicApi