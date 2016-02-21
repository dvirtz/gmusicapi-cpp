#include "Module.h"
#include "RegisteredDevice.h"
#include "gmusicapiPath.h"
#include "Song.h"
#include "Playlist.h"
#include "RadioStation.h"
#include "SearchResults.h"
#include "Genre.h"
#include "typeConverters.h"
#include "SharedPlaylistInfo.h"

namespace GMusicApi
{

std::once_flag Module::m_onceFlag;

Module::Module()
    : PythonHelper::ModuleBase("gmusicapi", gmusicapi_path)
{
    // execute this code only once
    std::call_once(m_onceFlag, []()
    {
        Module::registerTypeConverters();
    });
}

void Module::registerTypeConverters()
{
    namespace bp = boost::python;
    namespace ph = PythonHelper;

    ph::PyToCppConverter<bp::dict, UploadResult>::registerConverter();
    ph::PyToCppConverter<bp::tuple, UploadResult::value_type>::registerConverter();
    ph::PyToCppConverter<bp::tuple, UploadResults>::registerConverter();
    ph::PyToCppConverter<bp::tuple, DownloadResult>::registerConverter();
    ph::PyToCppConverter<bp::str, BinaryStream>::registerConverter();
    ph::PyToCppConverter<bp::tuple, DownloadInfo>::registerConverter();

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
    registerSharedPlaylistInfoConverters();
}

} // namespace GMusicApi
