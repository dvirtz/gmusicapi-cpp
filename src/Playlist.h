#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
#include <boost/optional.hpp>
MSC_RESTORE_WARNINGS
#include "GeneratedRange.h"
#include "genDefs.h"
#include "typeConverters.h"
#include <string>
#include <vector>

GMUSICAPI_DEFINE_STRUCT(
    (GMusicApi), PlaylistEntry,
    (std::string, kind)
    (bool, deleted)
    (GMusicApi::identifier, trackId)
    (GMusicApi::timestamp, lastModifiedTimestamp)
    (GMusicApi::identifier, playlistId)
    (GMusicApi::identifier, clientId)
    (std::string, absolutePosition)
    (std::string, source)
    (GMusicApi::timestamp, creationTimestamp)
    (GMusicApi::identifier, id)
    )

GMUSICAPI_DEFINE_STRUCT(
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

inline void registerPlaylistConverters()
{
    namespace bp = boost::python;
    // Python to C++ converters
    PyToCppConverter<bp::dict, Playlist>::registerConverter();
    PySequenceToCppContainerConverter<PlaylistRange>::registerConverter();
    PyGeneratorToGeneratedRangeConverter<Playlist>::registerConverter();
    PyToCppConverter<bp::dict, PlaylistEntry>::registerConverter();
    PySequenceToCppContainerConverter<std::vector<PlaylistEntry>>::registerConverter();

    // C++ to Python converters
    bp::to_python_converter<PlaylistEntry, StructToPyDictConverter<PlaylistEntry>>();
    bp::to_python_converter<boost::optional<PlaylistEntry>, BoostOptionalToPyConverter<PlaylistEntry>>();
}

} // namespace GMusicApi