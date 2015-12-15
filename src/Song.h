#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "AlbumArt.h"
#include "GeneratedRange.h"
#include "genDefs.h"
#include "typeConverters.h"
#include <string>
#include <vector>

BOOST_FUSION_DEFINE_STRUCT(
    (GMusicApi), Song,
    (std::string, comment)
    (std::string, rating)
    (std::vector<GMusicApi::AlbumArt>, albumArtRef)
    (std::vector<GMusicApi::identifier>, artistId)
    (std::string, composer)
    (int, year)
    (GMusicApi::timestamp, creationTimestamp)
    (GMusicApi::identifier, id)
    (std::string, album)
    (int, totalDiscCount)
    (std::string, title)
    (GMusicApi::timestamp, recentTimestamp)
    (std::string, albumArtist)
    (int, trackNumber)
    (int, discNumber)
    (bool, deleted)
    (GMusicApi::identifier, nid)
    (int, totalTrackCount)
    (std::string, estimatedSize)
    (GMusicApi::identifier, albumId)
    (int, beatsPerMinute)
    (std::string, genre)
    (int, playCount)
    (std::vector<GMusicApi::AlbumArt>, artistArtRef)
    (std::string, kind)
    (std::string, artist)
    (GMusicApi::timestamp, lastModifiedTimestamp)
    (GMusicApi::identifier, clientId)
    (GMusicApi::timestamp, durationMillis)
    )

namespace GMusicApi
{

using SongRange = GeneratedRange<Song>;

inline void registerSongConverters()
{
    namespace bp = boost::python;
    // Python to C++ converters
    PySequenceToCppContainerConverter<SongRange>::registerConverter();
    PyGeneratorToGeneratedRangeConverter<Song>::registerConverter();
    PyToCppConverter<bp::dict, Song>::registerConverter();
    PySequenceToCppContainerConverter<std::vector<Song>>::registerConverter();

    // C++ to Python converters
    bp::to_python_converter<std::vector<Song>, CppContainerToPyListConverter<std::vector<Song>>>();
    bp::to_python_converter<Song, StructToPyDictConverter<Song>>();
}

} // namespace GMusicApi