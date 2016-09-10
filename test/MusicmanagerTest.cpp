#include "catch.hpp"
#include "TestFixture.h"
#include "gmusicapi/Musicmanager.h"
#include "testFiles.h"
#include "gmusicapi/Mobileclient.h"
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

    class Utils : public PythonHelper::ModuleBase
    {
    public:
        Utils()
            : PythonHelper::ModuleBase("gmusicapi.utils.utils", gmusicapi_path)
        {}

        /*!
        Return the mac address interpretation of num, in the form eg '00:11:22:33:AA:BB'.

        \param num      a 48-bit integer (eg from uuid.getnode)
        \param splitter a string to join the hex pairs with
        */
        std::string create_mac_string(uint64_t num, const std::string& splitter = ":")
        {
            return callGlobalMethod<std::string>("create_mac_string", num, splitter);
        }
    };

public:
    MusicmanagerTestFixture()
        : m_mm(m_module)
    {
        m_credentials = m_protocol.credentials_from_refresh_token(gm_refresh);
    }
 
    bool login()
    {
        if (gm_android_id.empty())
	{
            return m_mm.login(m_credentials);        
        }
        else
        {
            Utils utils;
            auto mac = utils.create_mac_string(std::stoull(gm_android_id, nullptr, 16));
            return m_mm.login(m_credentials, mac);
        }
    }

protected:
    Musicmanager m_mm;
    MusicmanagerProtocol m_protocol;
    OAuth2Credentials m_credentials;
};

TEST_CASE_METHOD(MusicmanagerTestFixture, "Musicmanager login", "[Musicmanager]")
{
    REQUIRE(login());
    REQUIRE(m_mm.logout());
}

TEST_CASE_METHOD(MusicmanagerTestFixture, "Upload and Download", "[Musicmanager]")
{
    login();

    Mobileclient mc(m_module);
    mc.login(gm_user, gm_pass, gm_android_id);

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

