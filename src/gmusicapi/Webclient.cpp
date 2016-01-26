#include "Webclient.h"
#include "Webclient.h"
#include "Webclient.h"
#include "Webclient.h"
#include "Webclient.h"
#include "Webclient.h"
#include "Webclient.h"
#include "Webclient.h"
#include "Webclient.h"
#include "Webclient.h"
#include "Webclient.h"

namespace GMusicApi
{

Webclient::Webclient(bool debug_logging, bool validate, bool verify_ssl)
    : ClientBase("Webclient", debug_logging, validate, verify_ssl)
{}

bool Webclient::login(const std::string & email, const std::string & password)
{
    return callMethod<bool>("login", email, password);
}

bool Webclient::logout()
{
    return callMethod<bool>("logout");
}

DownloadInfo Webclient::get_song_download_info(const identifier & song_id)
{
    return callMethod<DownloadInfo>("get_song_download_info", song_id);
}

BinaryStream Webclient::get_stream_audio(const identifier & song_id, 
                                         const boost::optional<bool>& use_range_header)
{
    return callMethod<BinaryStream>("get_stream_audio", song_id, use_range_header);
}

std::vector<std::string> Webclient::get_stream_urls(const identifier & song_id)
{
    return callMethod<std::vector<std::string>>("get_stream_urls", song_id);
}

identifiers Webclient::report_incorrect_match(const identifiers & song_ids)
{
    return callMethod<identifiers>("report_incorrect_match", song_ids);
}

std::string Webclient::upload_album_art(const identifiers & song_ids, const std::string & image_filepath)
{
    return callMethod<std::string>("upload_album_art", song_ids, image_filepath);
}

SharedPlaylistInfo Webclient::get_shared_playlist_info(const std::string & share_token)
{
    return callMethod<SharedPlaylistInfo>("get_shared_playlist_info", share_token);
}


} // namespace GMusicApi
