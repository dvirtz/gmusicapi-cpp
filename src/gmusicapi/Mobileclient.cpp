#include "Mobileclient.h"
#include "utility.h"
#include <iostream>
#include <regex>

namespace py = boost::python;

namespace GMusicApi
{

Mobileclient::Mobileclient(bool debug_logging /*= true*/, 
                           bool validate /*= true*/, 
                           bool verify_ssl /*= true*/)
    : ClientBase("Mobileclient", debug_logging, validate, verify_ssl)
{}

bool Mobileclient::login(const std::string & email, const std::string & password, const identifier & android_id) const
{
    if (android_id.empty())
    {
        auto fromMacAddress = getMember("FROM_MAC_ADDRESS");
        return callMethod<bool>("login", email, password, fromMacAddress);
    }

    return callMethod<bool>("login", email, password, android_id);
}

bool Mobileclient::logout() const
{
    return callMethod<bool>("logout");
}

SongRange Mobileclient::get_all_songs(bool incremental, bool include_deleted)
{
    return callMethod<SongRange>("get_all_songs", incremental, include_deleted);
}

std::string Mobileclient::get_stream_url(const identifier & song_id,
                                         const boost::optional<identifier>& device_id,
                                         SongQuality quality)
{
    auto dev_id = device_id;
    // Omit '0x' from the start of the string if present.
    if (dev_id)
    {
        dev_id = std::regex_replace(*dev_id, std::regex("0x(.*)"), "$1");
    }
    return callMethod<std::string>("get_stream_url", song_id, dev_id, songQualityName(quality));
}

std::vector<RegisteredDevice> Mobileclient::get_registered_devices()
{
    return callMethod<std::vector<RegisteredDevice>>("get_registered_devices");
}

identifiers Mobileclient::change_song_metadata(const std::vector<Song>& songs)
{
    return callMethod<identifiers>("change_song_metadata", songs);
}

identifiers Mobileclient::delete_songs(const identifiers& song_ids)
{
    return callMethod<identifiers>("delete_songs", song_ids);
}

SongRange Mobileclient::get_promoted_songs()
{
    return callMethod<SongRange>("get_promoted_songs");
}

identifier Mobileclient::increment_song_playcount(const identifier & song_id,
                                                  int plays,
                                                  const boost::optional<boost::posix_time::ptime>& playtime)
{
    return callMethod<identifier>("increment_song_playcount", song_id, plays, playtime);
}

PlaylistRange Mobileclient::get_all_playlists(bool incremental, bool include_deleted)
{
    return callMethod<PlaylistRange>("get_all_playlists", incremental, include_deleted);
}

PlaylistRange Mobileclient::get_all_user_playlist_contents()
{
    return callMethod<PlaylistRange>("get_all_user_playlist_contents");
}

identifier Mobileclient::create_playlist(const std::string & name,
                                         const std::string & description,
                                         bool isPublic)
{
    return callMethod<identifier>("create_playlist", name, description, isPublic);
}

identifier Mobileclient::delete_playlist(const identifier& playlist_id)
{
    return callMethod<identifier>("delete_playlist", playlist_id);
}

identifier Mobileclient::edit_playlist(const identifier & playlist_id,
                                       const boost::optional<std::string>& new_name,
                                       const boost::optional<std::string>& new_description,
                                       const boost::optional<bool>& isPublic)
{
    return callMethod<identifier>("edit_playlist", playlist_id, new_name, new_description, isPublic);
}

identifiers Mobileclient::add_songs_to_playlist(const identifier & playlist_id, const identifiers& song_ids)
{
    return callMethod<identifiers>("add_songs_to_playlist", playlist_id, song_ids);
}

identifiers Mobileclient::reorder_playlist_entry(const PlaylistEntry & entry,
                                                 const boost::optional<PlaylistEntry>& to_follow_entry,
                                                 const boost::optional<PlaylistEntry>& to_precede_entry)
{
    return callMethod<identifiers>("reorder_playlist_entry", entry, to_follow_entry, to_precede_entry);
}

identifiers Mobileclient::remove_entries_from_playlist(const identifiers & entry_ids)
{
    return callMethod<identifiers>("remove_entries_from_playlist", entry_ids);
}

PlaylistRange Mobileclient::get_shared_playlist_contents(const std::string & share_token)
{
    return callMethod<PlaylistRange>("get_shared_playlist_contents", share_token);
}

RadioStationRange Mobileclient::get_all_stations(bool incremental, 
                                                 bool include_deleted, 
                                                 const boost::optional<boost::posix_time::ptime>& updated_after)
{
    return callMethod<RadioStationRange>("get_all_stations", incremental, include_deleted, updated_after);
}

SongRange Mobileclient::get_station_tracks(identifier station_id, 
                                           size_t num_tracks, 
                                           const boost::optional<identifiers>& recently_played_ids)
{
    return callMethod<SongRange>("get_station_tracks", station_id, num_tracks, recently_played_ids);
}

identifier Mobileclient::create_station(const std::string & name, 
                                        const boost::optional<identifier>& track_id, 
                                        const boost::optional<identifier>& artist_id, 
                                        const boost::optional<identifier>& album_id, 
                                        const boost::optional<identifier>& genre_id)
{
    return callMethod<identifier>("create_station", name, track_id, artist_id, album_id, genre_id);
}

identifiers Mobileclient::delete_stations(const identifiers & station_ids)
{
    return callMethod<identifiers>("delete_stations", station_ids);
}

SearchResults Mobileclient::search_all_access(const std::string & query, size_t max_results)
{
    return callMethod<SearchResults>("search_all_access", query, max_results);
}

identifier Mobileclient::add_aa_track(const identifier& aa_song_id)
{
    return callMethod<identifier>("add_aa_track", aa_song_id);
}


Artist Mobileclient::get_artist_info(const identifier& artist_id,
                                     bool include_albums,
                                     size_t max_top_tracks,
                                     size_t max_rel_artist)
{
    return callMethod<Artist>("get_artist_info", artist_id, include_albums, max_top_tracks, max_rel_artist);
}

Album Mobileclient::get_album_info(const identifier & album_id, bool include_tracks)
{
    return callMethod<Album>("get_album_info", album_id, include_tracks);
}

Song Mobileclient::get_track_info(const identifier & store_track_id)
{
    return callMethod<Song>("get_track_info", store_track_id);
}

GenreRange Mobileclient::get_genres(const boost::optional<identifier>& parent_genre_id)
{
    return callMethod<GenreRange>("get_genres", parent_genre_id);
}

} // namespace GMusicApi
