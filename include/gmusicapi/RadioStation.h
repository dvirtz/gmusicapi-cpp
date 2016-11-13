#pragma once

#include "PythonHelper/utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "gmusicapi/genDefs.h"
#include "PythonHelper/typeConverters.h"

GMUSICAPI_DEFINE_STRUCT(
    (GMusicApi), RadioSeed,
    (std::string, kind)
    (GMusicApi::identifier, albumId)
    (GMusicApi::identifier, artistId)
    (GMusicApi::identifier, genreId)
    (GMusicApi::identifier, trackId)
    (GMusicApi::identifier, trackLockerId)
    )

GMUSICAPI_DEFINE_STRUCT(
    (GMusicApi), RadioStation,
    (std::string, imageUrl)
    (std::string, kind)
    (std::string, name)
    (bool, deleted)
    (GMusicApi::timestamp, lastModifiedTimestamp)
    (GMusicApi::timestamp, recentTimestamp)
    (GMusicApi::identifier, clientId)
    (GMusicApi::RadioSeed, seed)
    (GMusicApi::identifier, id)
    )

namespace GMusicApi
{

using RadioStationRange = GeneratedRange<RadioStation>;

} // namespace GMusicApi