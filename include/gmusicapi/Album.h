#pragma once

#include "PythonHelper/utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "gmusicapi/genDefs.h"
#include "PythonHelper/typeConverters.h"

GMUSICAPI_DEFINE_STRUCT(
    (GMusicApi), Album,
    (std::string, albumArtRef)
    (std::string, albumArtist)
    (GMusicApi::identifier, albumId)
    (std::string, artist)
    (GMusicApi::identifiers, artistId)
    (std::string, kind)
    (std::string, name)
    (int, year)
    (std::vector<GMusicApi::Song>, tracks)
    )