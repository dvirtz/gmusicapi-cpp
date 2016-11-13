#pragma once

#include "PythonHelper/utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "gmusicapi/genDefs.h"
#include "gmusicapi/Album.h"
#include "gmusicapi/Artist.h"
#include "gmusicapi/Song.h"
#include "gmusicapi/Playlist.h"
#include "PythonHelper/typeConverters.h"

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
