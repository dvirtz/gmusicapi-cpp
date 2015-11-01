#include "utility.h"
#include "catch.hpp"
#include "Mobileclient.h"
#include "userCredentials.h"
#include "Song.h"

using namespace GMusicApi;

TEST_CASE("Mobileclient constructed", "[Mobileclient]")
{
	REQUIRE_NOTHROW(Mobileclient m);
}

TEST_CASE("Mobileclient login", "[Mobileclient]")
{
	Mobileclient m;

	SECTION("empty login failes")
	{
		REQUIRE_FALSE(m.login("", ""));
	}

	SECTION("login succeeds")
	{
		REQUIRE(m.login(gm_user, gm_pass));

		SECTION("song list not empty")
		{
			SECTION("incremental")
			{
				auto songs = m.get_all_songs(true);
				REQUIRE_FALSE(songs.empty());

				SECTION("registered devices list not empty")
				{
					auto registeredDevices = m.get_registered_devices();
					REQUIRE_FALSE(registeredDevices.empty());

					SECTION("stream url not empty")
					{
						auto song = songs.front();
						auto registeredDeviceIt 
							= std::find_if(registeredDevices.begin(), registeredDevices.end(), [](const RegisteredDevice& device)
						{
							return device.m_type == "ANDROID";
						});
						REQUIRE(registeredDeviceIt != registeredDevices.end());
						auto streamUrl = m.get_stream_url(song.m_id, registeredDeviceIt->m_id);
						REQUIRE_FALSE(streamUrl.empty());
					}
				}

				SECTION("can change song metadata")
				{
					auto song = songs.front();
					song.m_rating = "5";
					m.change_song_metadata({ song });
					REQUIRE(m.get_all_songs(true).front().m_rating == "5");
				}
			}
			SECTION("non incremental")
			{
				REQUIRE_FALSE(m.get_all_songs().empty());
			}
		}
	}

	SECTION("logout succeeds")
	{
		REQUIRE(m.logout());
	}
}