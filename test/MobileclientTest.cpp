#include "utility.h"
#include "catch.hpp"
#include "Mobileclient.h"
#include "userCredentials.h"
#include "Song.h"
#include "Playlist.h"
#include <boost/range.hpp>
#include <boost/range/algorithm.hpp>

using namespace GMusicApi;
using namespace PythonHelper;

TEST_CASE("Mobileclient constructed", "[Mobileclient]")
{
	REQUIRE_NOTHROW(Mobileclient mc);
}

TEST_CASE("Mobileclient login", "[Mobileclient]")
{
    Mobileclient mc;

    SECTION("empty login failes")
    {
        REQUIRE_FALSE(mc.login("", ""));
    }

    SECTION("login succeeds")
    {
        REQUIRE(mc.login(gm_user, gm_pass));

        SECTION("logout succeeds")
        {
            REQUIRE(mc.logout());
        }
    }
}

TEST_CASE("Mobileclient song list sanity", "[Mobileclient]")
{
    Mobileclient mc;
    mc.login(gm_user, gm_pass);

    auto incremental = mc.get_all_songs(true);
    auto non_incremental = mc.get_all_songs(false);

    REQUIRE(boost::equal(incremental, non_incremental));
}

TEST_CASE("registered devices list not empty", "[Mobileclient]")
{
    Mobileclient mc;
    mc.login(gm_user, gm_pass);

    REQUIRE_FALSE(mc.get_registered_devices().empty());
}

TEST_CASE("Song manipulation", "[Mobileclient]")
{
    Mobileclient mc;
    mc.login(gm_user, gm_pass);

    auto song = mc.get_all_songs(true).front();

    CAPTURE(song);
    
    auto songById = [&mc](const std::string& songId)
    {
        auto songs = mc.get_all_songs(true);
        return *std::find_if(songs.begin(), songs.end(), [songId](const Song& song)
        {
            return song.id == songId;
        });
    };

    auto songId = song.id;

    SECTION("stream url not empty")
    {
        REQUIRE_FALSE(mc.get_stream_url(songId).empty());
    }

    SECTION("can change song metadata")
    {
        auto checkRatingChange = [&songById, songId, &mc](const std::string& rating)
        {
            auto song = songById(songId);
            song.rating = rating;
            auto ids = mc.change_song_metadata({ song });
            REQUIRE(ids == std::vector<std::string>({ songId }));
            // refresh song
            song = songById(songId);
            REQUIRE(song.rating == rating);
        };
        checkRatingChange("5");
        checkRatingChange("0");
    }

    SECTION("can increment song play count")
    {
        auto currentPlayCount = song.playCount;
        auto playCountIncrement = 2;
        auto now = boost::posix_time::microsec_clock::universal_time();
        REQUIRE(mc.increment_song_playcount(songId, playCountIncrement, now) == songId);
        // refresh song
        song = songById(songId);
        REQUIRE(song.playCount == currentPlayCount + playCountIncrement);
    }
}

TEST_CASE("can change validate", "[Mobileclient]")
{
    Mobileclient mc;
    REQUIRE(mc.validate);
    mc.validate = false;
    REQUIRE_FALSE(mc.validate);
}

TEST_CASE("Promoted songs empty", "[Mobileclient][NoAllAccess]")
{
    Mobileclient mc;
    mc.login(gm_user, gm_pass);

    REQUIRE(mc.get_promoted_songs().empty());
}

TEST_CASE("Playlists sanity", "[Mobileclient]")
{
    Mobileclient mc;
    mc.login(gm_user, gm_pass);

    auto incremental = mc.get_all_playlists(true);
    auto non_incremental = mc.get_all_playlists();

    REQUIRE(boost::equal(incremental, non_incremental));
}

TEST_CASE("Playlists", "[Mobileclient]")
{
    Mobileclient mc;
    mc.login(gm_user, gm_pass);

    auto all_playlists_vec = toVector(mc.get_all_playlists());
    auto playlist_count = all_playlists_vec.size();
    auto list_id = mc.create_playlist("gmusicapi-cpp test playlist", "this is a new playlist");

    all_playlists_vec = toVector(mc.get_all_playlists());

    REQUIRE(all_playlists_vec.size() == playlist_count + 1);

    auto findPlaylist = [list_id](const std::vector<Playlist>& playlists)
    {
        return std::find_if(playlists.begin(), playlists.end(), [list_id](const Playlist& p) { return p.id == list_id; });
    };

    auto it = findPlaylist(all_playlists_vec);
    REQUIRE(it != all_playlists_vec.end());

    CAPTURE(*it);

    auto all_songs = mc.get_all_songs(true);
    auto& song = all_songs.front();

    CAPTURE(song);
    
    auto entry_ids = mc.add_songs_to_playlist(list_id, { song.id });
    REQUIRE(entry_ids.size() == 1);

    auto getPlaylist = [&mc, &findPlaylist]()
    {
        auto user_playlists = toVector(mc.get_all_user_playlist_contents());

        auto it = findPlaylist(user_playlists);
        REQUIRE(it != user_playlists.end());

        return *it;
    };


    auto playlist = getPlaylist();
    REQUIRE(playlist.tracks.size() == 1);

    auto& entry = playlist.tracks.front();
    REQUIRE(entry.id == entry_ids.front());
    REQUIRE(entry.trackId == song.id);
    REQUIRE(entry.playlistId == list_id);

    mc.add_songs_to_playlist(list_id, { song.id });

    playlist = getPlaylist();
    REQUIRE(playlist.tracks.size() == 2);

    entry_ids = identifiers{ playlist.tracks[0].id, playlist.tracks[1].id };
    // switch entry positions
    mc.reorder_playlist_entry(playlist.tracks[0], playlist.tracks[1]);

    playlist = getPlaylist();
    REQUIRE(playlist.tracks.size() == 2);
    REQUIRE(playlist.tracks[0].id == entry_ids[1]);
    REQUIRE(playlist.tracks[1].id == entry_ids[0]);

    auto entries_to_remove = identifiers{ entry_ids[0] };
    REQUIRE(entries_to_remove == mc.remove_entries_from_playlist(entries_to_remove));

    playlist = getPlaylist();
    REQUIRE(playlist.tracks.size() == 1);
    REQUIRE(playlist.tracks[0].id == entry_ids[1]);

    REQUIRE(list_id == mc.delete_playlist(list_id));
    all_playlists_vec = toVector(mc.get_all_playlists());
    REQUIRE(boost::size(all_playlists_vec) == playlist_count);
    REQUIRE(findPlaylist(all_playlists_vec) == all_playlists_vec.end());
}

TEST_CASE("Radio stations sanity", "[Mobileclient][.All Access]")
{
    Mobileclient mc;
    mc.login(gm_user, gm_pass);

    auto incremental = mc.get_all_stations(true);
    auto non_incremental = mc.get_all_stations(false);

    REQUIRE(boost::equal(incremental, non_incremental));
}

TEST_CASE("methods throw without AllAccess", "[Mobileclient]")
{
    Mobileclient mc;
    mc.login(gm_user, gm_pass);

    REQUIRE_THROWS_AS(mc.get_artist_info(""), std::runtime_error);
    REQUIRE_THROWS_AS(mc.get_album_info(""), std::runtime_error);
    REQUIRE_THROWS_AS(mc.get_track_info(""), std::runtime_error);
    REQUIRE_THROWS_AS(mc.get_genres(), std::runtime_error);
    REQUIRE_THROWS_AS(mc.search_all_access(""), std::runtime_error);
}