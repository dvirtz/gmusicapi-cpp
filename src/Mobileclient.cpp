#include "Mobileclient.h"
#include "utility.h"
#include <iostream>
#include <regex>

namespace py = boost::python;

namespace GMusicApi
{

Mobileclient::Mobileclient(bool debug_logging /*= true*/, bool validate /*= true*/, bool verify_ssl /*= true*/)
	: ClientBase("Mobileclient", debug_logging, validate, verify_ssl)
{
}

bool Mobileclient::login(const std::string & email, const std::string & password, const std::string & android_id) const
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

std::string Mobileclient::get_stream_url(const std::string & song_id, 
										 const std::string & device_id, 
										 SongQuality quality)
{
	// according to boost::python documentation at 
	// http://www.boost.org/doc/libs/1_38_0/libs/python/doc/v2/callbacks.html#argument_handling
	// nullptr arguments will result in None object being passed to Python
	if (device_id.empty())
		return callMethod<std::string>("get_stream_url", song_id, nullptr, songQualityName(quality));

	// Omit '0x' from the start of the string if present.
	auto device_id_without_prefix = std::regex_replace(device_id, std::regex("0x(.*)"), "$1");
	return callMethod<std::string>("get_stream_url", song_id, device_id_without_prefix, songQualityName(quality));
}

std::vector<RegisteredDevice> Mobileclient::get_registered_devices()
{
	return callMethod<std::vector<RegisteredDevice>>("get_registered_devices");
}

std::vector<std::string> Mobileclient::change_song_metadata(const std::vector<Song>& songs)
{
	return callMethod<std::vector<std::string>>("change_song_metadata", songs);
}

std::vector<std::string> Mobileclient::delete_songs(const std::vector<std::string> song_ids)
{
    return callMethod<std::vector<std::string>>("delete_songs", song_ids);
}

SongRange Mobileclient::get_promoted_songs()
{
    return callMethod<SongRange>("get_promoted_songs");
}

std::string Mobileclient::increment_song_playcount(const std::string & song_id, 
                                                   int plays, 
                                                   const boost::posix_time::ptime& playtime)
{
    return callMethod<std::string>("increment_song_playcount", song_id, plays, playtime);
}

PlaylistRange Mobileclient::get_all_playlists(bool incremental, bool include_deleted)
{
    return callMethod<PlaylistRange>("get_all_playlists", incremental, include_deleted);
}

} // namespace GMusicApi
