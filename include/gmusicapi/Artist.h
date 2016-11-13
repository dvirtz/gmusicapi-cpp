#pragma once

#include "PythonHelper/utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "gmusicapi/genDefs.h"

GMUSICAPI_DEFINE_STRUCT(
    (GMusicApi), Artist,
    (std::vector<GMusicApi::Album>, albums)
    (std::string, artistArtRef)
    (GMusicApi::identifier, artistId)
    (std::string, kind)
    (std::string, name)
    (std::vector<GMusicApi::Artist>, related_artists)
    (std::vector<GMusicApi::Song>, topTracks)
    (int, total_albums)
    )
