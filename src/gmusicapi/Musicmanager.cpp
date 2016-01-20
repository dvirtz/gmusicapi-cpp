#include "Musicmanager.h"

namespace GMusicApi
{

Musicmanager::Musicmanager(bool debug_logging, 
                           bool validate, 
                           bool verify_ssl)
    : ClientBase("Musicmanager", debug_logging, validate, verify_ssl)
{}

void Musicmanager::perform_oauth(const boost::optional<std::string>& storage_filepath, 
                                 bool open_browser)
{
    callStaticMethod<void>("Musicmanager", "perform_oauth", storage_filepath, open_browser);
}

bool Musicmanager::login(const std::string& oauth_credentials, 
                         const boost::optional<identifier>& uploader_id, 
                         const boost::optional<std::string>& uploader_name)
{
    return callMethod<bool>("login", oauth_credentials, uploader_id, uploader_name);
}

bool Musicmanager::logout(bool revoke_oauth)
{
    return callMethod<bool>("logout", revoke_oauth);
}

UploadResults Musicmanager::upload(const std::vector<std::string>& filepaths, 
                          const boost::variant<int, std::string>& transcode_quality, 
                          bool enable_matching)
{
    return callMethod<UploadResults>("upload", filepaths, transcode_quality, enable_matching);
}

SongRange Musicmanager::get_uploaded_songs(bool incremental)
{
    return callMethod<SongRange>("get_uploaded_songs", incremental);
}

DownloadResult Musicmanager::download_song(const identifier & song_id)
{
    return callMethod<DownloadResult>("download_song", song_id);
}

} // namespace GMusicApi
