#include "Module.h"
#include "RegisteredDevice.h"
#include "gmusicapiPath.h"
#include "Song.h"
#include "typeConverters.h"
#include "Playlist.h"
#include "RadioStation.h"
#include "SearchResults.h"
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
	PySequenceToCppContainerConverter<std::vector<std::string>>::registerConverter();
	PyToCppConverter<str, std::string>::registerConverter();

	// C++ to Python converters
	to_python_converter<nullptr_t, NullptrToNoneConverter>();
	to_python_converter<std::vector<std::string>, CppContainerToPyListConverter<std::vector<std::string>>>();
    to_python_converter<boost::posix_time::ptime, BoostPTimeToPyDateTimeConverter>();
    to_python_converter<boost::optional<std::string>, BoostOptionalToPyConverter<std::string>>();
    to_python_converter<boost::optional<bool>, BoostOptionalToPyConverter<bool>>();
    to_python_converter<boost::optional<boost::posix_time::ptime>, BoostOptionalToPyConverter<boost::posix_time::ptime>>();
    to_python_converter<boost::optional<identifiers>, BoostOptionalToPyConverter<identifiers>>();

    //call other registeration methods
    registerSongConverters();
    registerAlbumArtConverters();
    registerRegisteredDeviceConverters();
    registerPlaylistConverters();
    registerRadioStationConverters();
    registerAlbumConverters();
    registerArtistConverters();
    registerSearchResultsConverters();
}

} // namespace GMusicApi