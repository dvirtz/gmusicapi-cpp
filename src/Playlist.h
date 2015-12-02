#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "GeneratedRange.h"
#include "genDefs.h"
#include <string>
#include <vector>

BOOST_FUSION_DEFINE_STRUCT(
    (GMusicApi), PlaylistEntry,
    (std::string, kind)
    (bool,  deleted)
    (GMusicApi::identifier, trackId)
    (GMusicApi::timestamp, lastModifiedTimestamp)
    (GMusicApi::identifier, playlistId)
    (GMusicApi::identifier, clientId)
    (std::string, absolutePosition)
    (std::string, source)
    (GMusicApi::timestamp, creationTimestamp)
    (GMusicApi::identifier, id)
    )

BOOST_FUSION_DEFINE_STRUCT(
    (GMusicApi), Playlist,
    (std::string, type)
    (std::string, kind)
    (bool, deleted)
    (GMusicApi::timestamp, lastModifiedTimestamp)
    (GMusicApi::timestamp, recentTimestamp)
    (std::string, shareToken)
    (std::string, ownerProfilePhotoUrl)
    (std::string, ownerName)
    (bool, accessControlled)
    (GMusicApi::timestamp, creationTimestamp)
    (GMusicApi::identifier, id)
    (std::vector<GMusicApi::PlaylistEntry>, tracks)
    )

namespace GMusicApi
{

using PlaylistRange = GeneratedRange<Playlist>;

} // namespace GMusicApi