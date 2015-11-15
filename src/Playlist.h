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
    (GMusicApi), Playlist,
    (std::string, type)
    (std::string, kind)
    (bool, deleted)
    (std::string, lastModifiedTimestamp)
    (std::string, recentTimestamp)
    (std::string, shareToken)
    (std::string, ownerProfilePhotoUrl)
    (std::string, ownerName)
    (bool, accessControlled)
    (std::string, creationTimestamp)
    (std::string, id)
    )

namespace GMusicApi
{

using PlaylistRange = GeneratedRange<Playlist>;

} // namespace GMusicApi