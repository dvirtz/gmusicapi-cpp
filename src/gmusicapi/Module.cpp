#include "gmusicapi/Module.h"
#include "gmusicapi/RegisteredDevice.h"
#include "gmusicapi/Song.h"
#include "gmusicapi/Playlist.h"
#include "gmusicapi/RadioStation.h"
#include "gmusicapi/SearchResults.h"
#include "gmusicapi/Genre.h"
#include "PythonHelper/typeConverters.h"
#include "gmusicapi/SharedPlaylistInfo.h"
#include "gmusicapiPath.h"

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
#if PYTHON_MAJOR == 2
    ph::PyToCppConverter<bp::str, BinaryStream>::registerConverter();
#else
    ph::PyToCppConverter<bp::object, BinaryStream>::registerConverter();
#endif
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
