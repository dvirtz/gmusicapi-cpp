#include "catch.hpp"
#include "TestFixture.h"
#include "Musicmanager.h"
#include "testFiles.h"
#include "Mobileclient.h"
#include "userCredentials.h"
#include "gmusicapiPath.h"
#include <regex>

using namespace GMusicApi;

class MusicmanagerTestFixture : public TestFixture
{
    class MusicmanagerProtocol : public PythonHelper::ModuleBase
    {
    public:
        MusicmanagerProtocol()
            : PythonHelper::ModuleBase("gmusicapi.protocol.musicmanager", gmusicapi_path)
        {}

        OAuth2Credentials credentials_from_refresh_token(const std::string& token)
        {
            return callGlobalMethod<OAuth2Credentials>("credentials_from_refresh_token", token);
        }
    };

public:
    MusicmanagerTestFixture()
        : m_mm(m_module)
    {
        m_credentials = m_protocol.credentials_from_refresh_token(gm_refresh);
    }
 

protected:
    Musicmanager m_mm;
    MusicmanagerProtocol m_protocol;
    OAuth2Credentials m_credentials;
};

TEST_CASE_METHOD(MusicmanagerTestFixture, "Musicmanager login", "[Musicmanager]")
{
    REQUIRE(m_mm.login());
    REQUIRE(m_mm.logout());
}

TEST_CASE_METHOD(MusicmanagerTestFixture, "Upload and Download", "[Musicmanager]")
{
    m_mm.login(m_credentials);

    Mobileclient mc(m_module);
    mc.login(gm_user, gm_pass);

    UploadResult uploaded, matched, failed;
    std::tie(uploaded, matched, failed) = m_mm.upload({ audioTestPath });

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
        auto it = failed.find(audioTestPath);
        REQUIRE(it != failed.end());
        
        mc.delete_songs({ songId(it->second) });

        std::tie(uploaded, matched, failed) = m_mm.upload({ audioTestPath });
    }

    REQUIRE(uploaded.size() == 1);
    REQUIRE(matched.size() == 0);
    REQUIRE(failed.size() == 0);

    auto it = uploaded.find(audioTestPath);
    REQUIRE(it != uploaded.end());

    auto id = it->second;

    auto uploadedSongs = PythonHelper::toVector(m_mm.get_uploaded_songs());

    auto itt = std::find_if(uploadedSongs.begin(), uploadedSongs.end(), [id](const Song& song)
    {
        return song.id == id;
    });

    REQUIRE(itt != uploadedSongs.end());

    std::string filename;
    BinaryStream audio;
    std::tie(filename, audio) = m_mm.download_song(id);
    REQUIRE(filename.empty() == false);
    REQUIRE(audio.empty() == false);

    mc.delete_songs({ id });
}