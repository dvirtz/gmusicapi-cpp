#include "catch.hpp"
#include "Webclient.h"
#include "userCredentials.h"
#include "Mobileclient.h"
#include "Song.h"
#include "testFiles.h"

using namespace GMusicApi;

static Song getSong()
{
    Mobileclient mc;
    mc.login(gm_user, gm_pass);

    return *mc.get_all_songs().begin();
}

TEST_CASE("Webclient constructed", "[Webclient]")
{
	REQUIRE_NOTHROW(Webclient wc);
}

TEST_CASE("Webclient empty login fails", "[Webclient]")
{
    Webclient wc;
    REQUIRE_FALSE(wc.login("", ""));
}

TEST_CASE("Webclient login logout", "[Webclient]")
{
    Webclient wc;
    REQUIRE(wc.login(gm_user, gm_pass));
    REQUIRE(wc.logout());
}

TEST_CASE("download info received", "[Webclient]")
{
    Webclient wc;
    wc.login(gm_user, gm_pass);

    auto song = getSong();

    auto downloadInfo = wc.get_song_download_info(song.id);
    REQUIRE_FALSE(downloadInfo.first.empty());
    REQUIRE(downloadInfo.second <= 2);
}

TEST_CASE("audio stream received", "[Webclient]")
{
    Webclient wc;
    wc.login(gm_user, gm_pass);

    auto song = getSong();

    auto stream = wc.get_stream_audio(song.id);
    REQUIRE_FALSE(stream.empty());
}

TEST_CASE("stream URL received", "[Webclient]")
{
    Webclient wc;
    wc.login(gm_user, gm_pass);

    auto song = getSong();

    auto urls = wc.get_stream_urls(song.id);
    REQUIRE(urls.size() == 1);
}

TEST_CASE("report incorrect match doesn't throw", "[Webclient]")
{
    Webclient wc;
    wc.login(gm_user, gm_pass);

    auto song = getSong();
    auto ids = identifiers{ song.id };
    identifiers ret_ids;
    REQUIRE_NOTHROW(ret_ids = wc.report_incorrect_match(ids));
    REQUIRE(ret_ids == ids);
}

TEST_CASE("album art uploaded", "[Webclient]")
{
    Webclient wc;
    wc.login(gm_user, gm_pass);

    auto song = getSong();

    auto url = wc.upload_album_art({ song.id }, imageTestPath);
    REQUIRE_FALSE(url.empty());
}