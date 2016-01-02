#include "Module.h"
#include "RegisteredDevice.h"
#include "gmusicapiPath.h"
#include "Song.h"
#include "typeConverters.h"
#include "Playlist.h"
#include "RadioStation.h"
#include "SearchResults.h"
#include <iostream>
#include <mutex>

namespace GMusicApi
{

Module::Module()
    : PythonHelper::ModuleBase<Module>("gmusicapi", gmusicapi_path)
{
    registerTypeConverters();
}

void Module::registerTypeConverters()
{
    using namespace boost::python;

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