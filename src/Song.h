#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "AlbumArt.h"
#include "GeneratedRange.h"
#include <string>
#include <vector>

using GMAlbumArt = GMusicApi::AlbumArt;

BOOST_FUSION_DEFINE_STRUCT(
    (GMusicApi), Song,
    (std::string, comment)
    (std::string, rating)
    (std::vector<GMAlbumArt>, albumArtRef)
    (std::vector<std::string>, artistId)
    (std::string, composer)
    (int, year)
    (std::string, creationTimestamp)
    (std::string, id)
    (std::string, album)
    (int, totalDiscCount)
    (std::string, title)
    (std::string, recentTimestamp)
    (std::string, albumArtist)
    (int, trackNumber)
    (int, discNumber)
    (bool, deleted)
    (std::string, nid)
    (int, totalTrackCount)
    (std::string, estimatedSize)
    (std::string, albumId)
    (int, beatsPerMinute)
    (std::string, genre)
    (int, playCount)
    (std::vector<GMAlbumArt>, artistArtRef)
    (std::string, kind)
    (std::string, artist)
    (std::string, lastModifiedTimestamp)
    (std::string, clientId)
    (std::string, durationMillis)
    )

namespace GMusicApi
{

using SongRange = GeneratedRange<Song>;
using pyGeneratorToSongRangeConverter = pyGeneratorToGeneratedRangeConverter<Song>;

} // namespace GMusicApi