#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "genDefs.h"
#include "Album.h"
#include "Artist.h"
#include "Song.h"
#include "Playlist.h"
#include "typeConverters.h"

#define DEFINE_SEARCH_HIT_STRUCT(Type, name) \
BOOST_FUSION_DEFINE_STRUCT( \
    (GMusicApi), Type ## Hit, \
    (GMusicApi:: Type , name ) \
    (bool, best_result) \
    (double, score) \
    (std::string, type) \
    )

DEFINE_SEARCH_HIT_STRUCT(Album, album);
DEFINE_SEARCH_HIT_STRUCT(Artist, artist);
DEFINE_SEARCH_HIT_STRUCT(Song, track);
DEFINE_SEARCH_HIT_STRUCT(Playlist, playlist);

#undef DEFINE_SEARCH_HIT_STRUCT

BOOST_FUSION_DEFINE_STRUCT(
    (GMusicApi), SearchResults,
    (std::vector<GMusicApi::AlbumHit>, album_hits)
    (std::vector<GMusicApi::ArtistHit>, artist_hits)
    (std::vector<GMusicApi::SongHit>, song_hits)
    (std::vector<GMusicApi::PlaylistHit>, playlist_hits)
    )

namespace GMusicApi
{

inline void registerSearchResultsConverters()
{
    namespace bp = boost::python;
    // Python to C++ converters
    PyToCppConverter<bp::dict, AlbumHit>::registerConverter();
    PyToCppConverter<bp::dict, ArtistHit>::registerConverter();
    PyToCppConverter<bp::dict, SongHit>::registerConverter();
    PyToCppConverter<bp::dict, PlaylistHit>::registerConverter();
    PyToCppConverter<bp::dict, SearchResults>::registerConverter();
    PySequenceToCppContainerConverter<std::vector<AlbumHit>>::registerConverter();
    PySequenceToCppContainerConverter<std::vector<ArtistHit>>::registerConverter();
    PySequenceToCppContainerConverter<std::vector<SongHit>>::registerConverter();
    PySequenceToCppContainerConverter<std::vector<PlaylistHit>>::registerConverter();

    // C++ to Python converters
}

} // namespace GMusicApi