#include "utility.h"
#include "catch.hpp"
#include "Mobileclient.h"
#include "userCredentials.h"
#include "Song.h"

using namespace GMusicApi;

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

TEST_CASE("Mobileclient song list", "[Mobileclient]")
{
    Mobileclient mc;
    mc.login(gm_user, gm_pass);

    SECTION("incremental")
    {
        auto songs = mc.get_all_songs(true);
        REQUIRE_FALSE(songs.empty());
    }

    SECTION("non incremental")
    {
        REQUIRE_FALSE(mc.get_all_songs().empty());
    }
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
        auto registeredDevices = mc.get_registered_devices();

        auto registeredDeviceIt
            = std::find_if(registeredDevices.begin(), registeredDevices.end(), [](const RegisteredDevice& device)
        {
            return device.type == "ANDROID";
        });
        REQUIRE(registeredDeviceIt != registeredDevices.end());
        REQUIRE_FALSE(mc.get_stream_url(songId, registeredDeviceIt->id).empty());
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
        //std::string in; std::cout << "ready\n"; std::cin >> in;
        REQUIRE(mc.increment_song_playcount(songId, playCountIncrement, now) == songId);
        // refresh song
        song = songById(songId);
        REQUIRE(song.playCount == currentPlayCount + playCountIncrement);
    }
}

TEST_CASE("can change validate", "[Mobileclient]")
{
    Mobileclient mc;
    REQUIRE(mc.validate());
    mc.set_validate(false);
    REQUIRE_FALSE(mc.validate());
}

TEST_CASE("Promoted songs empty", "[Mobileclient][NoAllAccess]")
{
    Mobileclient mc;
    mc.login(gm_user, gm_pass);

    REQUIRE(mc.get_promoted_songs().empty());
}