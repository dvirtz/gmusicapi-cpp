#include "Module.h"
#include "RegisteredDevice.h"
#include "gmusicapiPath.h"
#include "Song.h"
#include "Playlist.h"
#include "RadioStation.h"
#include "SearchResults.h"
#include "Genre.h"
#include "typeConverters.h"

namespace GMusicApi
{

Module::Module()
    : PythonHelper::ModuleBase<Module>("gmusicapi", gmusicapi_path)
{
    registerTypeConverters();
}

void Module::registerTypeConverters()
{
    namespace bp = boost::python;
    namespace ph = PythonHelper;

    ph::PyToCppConverter<bp::dict, UploadResult>::registerConverter();
    ph::PyToCppConverter<bp::tuple, UploadResult::value_type>::registerConverter();
    ph::PyToCppConverter<bp::tuple, UploadResults>::registerConverter();
    ph::PyToCppConverter<bp::tuple, DownloadResult>::registerConverter();

    bp::to_python_converter<boost::optional<identifiers>, ph::BoostOptionalToPyConverter<identifiers>>();

    //call other registeration methods
    registerSongConverters();
    registerAlbumArtConverters();
    registerRegisteredDeviceConverters();
    registerPlaylistConverters();
    registerRadioStationConverters();
    registerAlbumConverters();
    registerArtistConverters();
    registerGenreConverters();
    registerSearchResultsConverters();
}

} // namespace GMusicApi