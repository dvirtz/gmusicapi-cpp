#include "MobileClient.h"
#include "utility.h"
#include <iostream>
#include <regex>

namespace py = boost::python;

namespace GMusicApi
{

Mobileclient::Mobileclient(bool debug_logging /*= true*/, bool validate /*= true*/, bool verify_ssl /*= true*/)
	: GMusicApiUser("Mobileclient", debug_logging, validate, verify_ssl)
{
}


Mobileclient::~Mobileclient()
{
}

bool Mobileclient::login(const std::string & email, const std::string & password, const std::string & android_id) const
{
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


} // namespace GMusicApi
