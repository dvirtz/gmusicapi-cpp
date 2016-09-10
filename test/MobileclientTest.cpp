#include "TestFixture.h"
#include "PythonHelper/utility.h"
#include "catch.hpp"
#include "gmusicapi/Mobileclient.h"
#include "gmusicapi/Song.h"
#include "gmusicapi/Playlist.h"
#include <boost/range.hpp>
#include <boost/range/algorithm.hpp>

using namespace GMusicApi;
using namespace PythonHelper;

class MobileclientTestFixture: public TestFixture
{
public:
    MobileclientTestFixture()
        : m_mc(m_module)
    {}

    bool login()
    {
        return m_mc.login(gm_user, gm_pass, gm_android_id);
    }

    template<typename Func>
    void retry(Func&& f, size_t retries)
    {
        for (size_t i = 0; i < retries; ++i)
        {
            f();
        }
    }

protected:
    Mobileclient m_mc;
};

TEST_CASE_METHOD(MobileclientTestFixture, "Mobileclient login", "[Mobileclient]")
{
    SECTION("empty login failes")
    {
        REQUIRE_FALSE(m_mc.login("", "", gm_android_id));
    }

    SECTION("login succeeds")
    {
        REQUIRE(login());

        SECTION("logout succeeds")
        {
            REQUIRE(m_mc.logout());
        }
    }
}

TEST_CASE_METHOD(MobileclientTestFixture, "Mobileclient song list sanity", "[Mobileclient]")
{
    login();

    auto incremental = m_mc.get_all_songs(true);
    auto non_incremental = m_mc.get_all_songs(false);

    REQUIRE(boost::equal(incremental, non_incremental));
}

TEST_CASE_METHOD(MobileclientTestFixture, "registered devices list not empty", "[Mobileclient]")
{
    login();

    REQUIRE_FALSE(m_mc.get_registered_devices().empty());
}

TEST_CASE_METHOD(MobileclientTestFixture, "Song manipulation", "[Mobileclient]")
{
    login();

    auto song = m_mc.get_all_songs(true).front();

    CAPTURE(song);
    
    auto songById = [this](const std::string& songId)
    {
        auto songs = m_mc.get_all_songs(true);
        return *std::find_if(songs.begin(), songs.end(), [songId](const Song& song)
        {
            return song.id == songId;
        });
    };

    auto songId = song.id;

    SECTION("stream url not empty")
    {
        REQUIRE_FALSE(m_mc.get_stream_url(songId).empty());
    }

    SECTION("can change song metadata")
    {
        auto checkRatingChange = [&songById, songId, this](const std::string& rating)
        {
            auto song = songById(songId);
            song.rating = rating;
            auto ids = m_mc.change_song_metadata({ song });
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
        REQUIRE(m_mc.increment_song_playcount(songId, playCountIncrement, now) == songId);
        // refresh song
        song = songById(songId);
        REQUIRE(song.playCount == currentPlayCount + playCountIncrement);
    }
}

TEST_CASE_METHOD(MobileclientTestFixture, "can change validate", "[Mobileclient]")
{
    REQUIRE(m_mc.validate());
    m_mc.setValidate(false);
    REQUIRE_FALSE(m_mc.validate());
}

TEST_CASE_METHOD(MobileclientTestFixture, "Promoted songs empty", "[Mobileclient][NoAllAccess]")
{
    login();

    REQUIRE(m_mc.get_promoted_songs().empty());
}

TEST_CASE_METHOD(MobileclientTestFixture, "Playlists sanity", "[Mobileclient]")
{
    login();

    auto incremental = m_mc.get_all_playlists(true);
    auto non_incremental = m_mc.get_all_playlists();

    REQUIRE(boost::equal(incremental, non_incremental));
}

TEST_CASE_METHOD(MobileclientTestFixture, "Playlists", "[Mobileclient]")
{
    login();

    auto all_playlists_vec = toVector(m_mc.get_all_playlists());
    auto playlist_count = all_playlists_vec.size();
    auto list_id = m_mc.create_playlist("gmusicapi-cpp test playlist", "this is a new playlist");

    all_playlists_vec = toVector(m_mc.get_all_playlists());

    REQUIRE(all_playlists_vec.size() == playlist_count + 1);

    auto findPlaylist = [list_id](const std::vector<Playlist>& playlists)
    {
        return std::find_if(playlists.begin(), playlists.end(), [list_id](const Playlist& p) { return p.id == list_id; });
    };

    auto it = findPlaylist(all_playlists_vec);
    REQUIRE(it != all_playlists_vec.end());

    CAPTURE(*it);

    auto all_songs = m_mc.get_all_songs(true);
    auto& song = all_songs.front();

    CAPTURE(song);
    
    auto entry_ids = m_mc.add_songs_to_playlist(list_id, { song.id });
    REQUIRE(entry_ids.size() == 1);

    auto getPlaylist = [this, &findPlaylist]()
    {
        auto user_playlists = toVector(m_mc.get_all_user_playlist_contents());

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

    m_mc.add_songs_to_playlist(list_id, { song.id });

    playlist = getPlaylist();
    REQUIRE(playlist.tracks.size() == 2);

    entry_ids = identifiers{ playlist.tracks[0].id, playlist.tracks[1].id };
    // switch entry positions
    m_mc.reorder_playlist_entry(playlist.tracks[0], playlist.tracks[1]);

    playlist = getPlaylist();
    REQUIRE(playlist.tracks.size() == 2);
    REQUIRE(playlist.tracks[0].id == entry_ids[1]);
    REQUIRE(playlist.tracks[1].id == entry_ids[0]);

    auto entries_to_remove = identifiers{ entry_ids[0] };
    REQUIRE(entries_to_remove == m_mc.remove_entries_from_playlist(entries_to_remove));

    playlist = getPlaylist();
    REQUIRE(playlist.tracks.size() == 1);
    REQUIRE(playlist.tracks[0].id == entry_ids[1]);

    REQUIRE(list_id == m_mc.delete_playlist(list_id));
    all_playlists_vec = toVector(m_mc.get_all_playlists());
    REQUIRE(boost::size(all_playlists_vec) == playlist_count);
    REQUIRE(findPlaylist(all_playlists_vec) == all_playlists_vec.end());
}

TEST_CASE_METHOD(MobileclientTestFixture, "Radio stations sanity", "[Mobileclient][.All Access]")
{
    login();

    auto incremental = m_mc.get_all_stations(true);
    auto non_incremental = m_mc.get_all_stations(false);

    REQUIRE(boost::equal(incremental, non_incremental));
}

TEST_CASE_METHOD(MobileclientTestFixture, "methods throw without AllAccess", "[Mobileclient]")
{
    login();

    REQUIRE_THROWS_AS(retry([this] { m_mc.get_artist_info(""); }, 5), std::runtime_error);
    REQUIRE_THROWS_AS(retry([this] { m_mc.get_album_info(""); }, 5), std::runtime_error);
    REQUIRE_THROWS_AS(retry([this] { m_mc.get_track_info(""); }, 5), std::runtime_error);
    //REQUIRE_THROWS_AS(retry([this] { m_mc.get_genres(); }, 5), std::runtime_error);
    //REQUIRE_THROWS_AS(retry([this] { m_mc.search_all_access(""); }, 5), std::runtime_error);
}
