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
GMUSICAPI_DEFINE_STRUCT( \
    (GMusicApi), Type ## Hit, \
    (GMusicApi:: Type , name ) \
    (bool, best_result) \
    (double, score) \
    (std::string, type) \
    )

DEFINE_SEARCH_HIT_STRUCT(Album, album)
DEFINE_SEARCH_HIT_STRUCT(Artist, artist)
DEFINE_SEARCH_HIT_STRUCT(Song, track)
DEFINE_SEARCH_HIT_STRUCT(Playlist, playlist)

#undef DEFINE_SEARCH_HIT_STRUCT

GMUSICAPI_DEFINE_STRUCT(
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
    namespace ph = PythonHelper;

    // Python to C++ converters
    ph::PyToCppConverter<bp::dict, AlbumHit>::registerConverter();
    ph::PyToCppConverter<bp::dict, ArtistHit>::registerConverter();
    ph::PyToCppConverter<bp::dict, SongHit>::registerConverter();
    ph::PyToCppConverter<bp::dict, PlaylistHit>::registerConverter();
    ph::PyToCppConverter<bp::dict, SearchResults>::registerConverter();
    ph::PySequenceToCppContainerConverter<std::vector<AlbumHit>>::registerConverter();
    ph::PySequenceToCppContainerConverter<std::vector<ArtistHit>>::registerConverter();
    ph::PySequenceToCppContainerConverter<std::vector<SongHit>>::registerConverter();
    ph::PySequenceToCppContainerConverter<std::vector<PlaylistHit>>::registerConverter();

    // C++ to Python converters
}

} // namespace GMusicApi
