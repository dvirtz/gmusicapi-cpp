#include "catch.hpp"
#include "Musicmanager.h"
#include "Clients.h"
#include "audiotestPath.h"
#include "Mobileclient.h"
#include "userCredentials.h"
#include <regex>

using namespace GMusicApi;

TEST_CASE("Musicmanager constructed", "[Musicmanager]")
{
    REQUIRE_NOTHROW(Musicmanager mm);
}

TEST_CASE("OAUTH_FILEPATH is not empty", "[Musicmanager]")
{
    std::string oauthFilepath;
    REQUIRE_NOTHROW(oauthFilepath = Clients::instance().OAUTH_FILEPATH);
    REQUIRE_FALSE(oauthFilepath.empty());
}

TEST_CASE("Login logout", "[Musicmanager]")
{
    Musicmanager mm;
    REQUIRE(mm.login());
    REQUIRE(mm.logout());
}

TEST_CASE("Upload and Download", "[Musicmanager]")
{
    Musicmanager mm;
    mm.login();

    Mobileclient mc;
    mc.login(gm_user, gm_pass);

    UploadResult uploaded, matched, failed;
    std::tie(uploaded, matched, failed) = mm.upload({ audiotestPath });

    // extract song id from string
    auto songId = [](const std::string& str)
    {
        std::regex idRe("\\((.*)\\)");
        std::smatch match;
        REQUIRE(std::regex_search(str, match, idRe));
        REQUIRE(match.size() > 1);

        return match.str(1);
    };

    // delete the song if it exists already because a previous test failed
    if (failed.size() == 1)
    {
        auto it = failed.find(audiotestPath);
        REQUIRE(it != failed.end());
        
        mc.delete_songs({ songId(it->second) });

        std::tie(uploaded, matched, failed) = mm.upload({ audiotestPath });
    }

    REQUIRE(uploaded.size() == 1);
    REQUIRE(matched.size() == 0);
    REQUIRE(failed.size() == 0);

    auto it = uploaded.find(audiotestPath);
    REQUIRE(it != uploaded.end());

    auto id = it->second;

    auto uploadedSongs = PythonHelper::toVector(mm.get_uploaded_songs());

    auto itt = std::find_if(uploadedSongs.begin(), uploadedSongs.end(), [id](const Song& song)
    {
        return song.id == id;
    });

    REQUIRE(itt != uploadedSongs.end());

    std::string filename, audio;
    std::tie(filename, audio) = mm.download_song(id);
    REQUIRE(filename.empty() == false);
    REQUIRE(audio.empty() == false);

    mc.delete_songs({ id });
}