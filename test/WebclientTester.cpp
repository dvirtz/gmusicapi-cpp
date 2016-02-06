#include "catch.hpp"
#include "TestFixture.h"
#include "Webclient.h"
#include "userCredentials.h"
#include "Mobileclient.h"
#include "Song.h"
#include "testFiles.h"

using namespace GMusicApi;

class WebclientTestFixture : public TestFixture
{
public:
    WebclientTestFixture()
        : m_wc(m_module)
    {}

protected:

    Song getSong()
    {
        Mobileclient mc(m_module);
        mc.login(gm_user, gm_pass);

        return *mc.get_all_songs().begin();
    }

    Webclient m_wc;
};

TEST_CASE_METHOD(WebclientTestFixture, "Webclient empty login fails", "[Webclient]")
{
    REQUIRE_FALSE(m_wc.login("", ""));
}

TEST_CASE_METHOD(WebclientTestFixture, "Webclient login logout", "[Webclient]")
{
    REQUIRE(m_wc.login(gm_user, gm_pass));
    REQUIRE(m_wc.logout());
}

TEST_CASE_METHOD(WebclientTestFixture, "download info received", "[Webclient]")
{
    m_wc.login(gm_user, gm_pass);

    auto song = getSong();

    auto downloadInfo = m_wc.get_song_download_info(song.id);
    REQUIRE_FALSE(downloadInfo.first.empty());
    REQUIRE(downloadInfo.second <= 2);
}

TEST_CASE_METHOD(WebclientTestFixture, "audio stream received", "[Webclient]")
{
    m_wc.login(gm_user, gm_pass);

    auto song = getSong();

    auto stream = m_wc.get_stream_audio(song.id);
    REQUIRE_FALSE(stream.empty());
}

TEST_CASE_METHOD(WebclientTestFixture, "stream URL received", "[Webclient]")
{
    m_wc.login(gm_user, gm_pass);

    auto song = getSong();

    auto urls = m_wc.get_stream_urls(song.id);
    REQUIRE(urls.size() == 1);
}

TEST_CASE_METHOD(WebclientTestFixture, "report incorrect match doesn't throw", "[Webclient]")
{
    m_wc.login(gm_user, gm_pass);

    auto song = getSong();
    auto ids = identifiers{ song.id };
    identifiers ret_ids;
    REQUIRE_NOTHROW(ret_ids = m_wc.report_incorrect_match(ids));
    REQUIRE(ret_ids == ids);
}

TEST_CASE_METHOD(WebclientTestFixture, "album art uploaded", "[Webclient]")
{
    m_wc.login(gm_user, gm_pass);

    auto song = getSong();

    auto url = m_wc.upload_album_art({ song.id }, imageTestPath);
    REQUIRE_FALSE(url.empty());
}