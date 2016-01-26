#pragma once
#include "ClientBase.h"
#include "genDefs.h"
#include "SharedPlaylistInfo.h"
#include "RegisteredDevice.h"

namespace GMusicApi
{

class Webclient : public ClientBase
{
public:
    /*!
    Constructor.

    \param	debug_logging	each Client has a `logger` member.
                            The logger is named `gmusicapi.<client class><client number>` and will propogate
                            to the gmusicapi root logger.
                            If this param is `true`, handlers will be configured to send this client's debug
                            log output to disk, with warnings and above printed to stderr.
                            [Appdirs](https://pypi.python.org/pypi/appdirs) `user_log_dir` is used by default.
                            Users can run:

                                Utils u;
                                std::cout << u.log_filepath();

                            to see the exact location on their system.
                            If `false`, no handlers will be configured; users must create their own handlers.
                            Completely ignoring logging is dangerous and not recommended.
                            The Google Music protocol can change at any time;
                            if something were to go wrong, the logs would be necessary for recovery.
    \param	validate	    if `false`, do not validate server responses against known schemas.
                            This helps to catch protocol changes, but requires significant cpu work.
                            This arg is stored and can be safely modified at runtime by calling validate().
    \param	verify_ssl      if `false`, exceptions will not be raised if there are problems verifying SSL certificates.
                            Be wary of using this option; it's almost always better to fix the machine's SSL
                            configuration than to ignore errors.
    */
    Webclient(bool debug_logging = true,
              bool validate = true,
              bool verify_ssl = true);

    /*!
    Authenticates the webclient.
    Returns `true` on success, `false` on failure.

    \param email    eg `'test@gmail.com'` or just `'test'`.
    \param password the account's password password.
                    This is not stored locally, and is sent securely over SSL.
                    App-specific passwords are not supported.

    Users who don't use two-factor auth will likely need to enable
    [less secure login](https://www.google.com/settings/security/lesssecureapps>).
    If this is needed, a warning will be logged during login (which will print to stderr
    in the default logging configuration).

    Users of two-factor authentication will need to set an application-specific password
    to log in.
    */
	bool login(const std::string& email, const std::string& password);

    /*!
    Forgets local authentication in this Api instance.
    Returns `true` on success.
    */
    bool logout();

    /*!
    Returns a pair: `{'<url>', <download count>}`.

    \param song_id  a single song id.

    `url` will be empty if the download limit is exceeded.

    GM allows 2 downloads per song. The download count may not always be accurate,
    and the 2 download limit seems to be loosely enforced.

    This call alone does not count towards a download -
    the count is incremented when `url` is retrieved.
    */
    DownloadInfo get_song_download_info(const identifier& song_id);

    /*!
    Returns a bytestring containing mp3 audio for this song.

    \param song_id: a single song id
    \param use_range_header: in some cases, an HTTP range header can be
        used to save some bandwidth.
        However, there's no guarantee that the server will respect it,
        meaning that the client may get back an unexpected response when
        using it.

        There are three possible values for this argument:
            * none: (default) send header; fix response locally on problems
            * true: send header; raise IOError on problems
            * false: do not send header
    */
    BinaryStream get_stream_audio(const identifier& song_id, 
                                  const boost::optional<bool>& use_range_header = {});


    /*!
    Returns a list of urls that point to a streamable version of this song.

    If you just need the audio and are ok with gmusicapi doing the download,
    consider using get_stream_audio() instead.
    This abstracts away the differences between different kinds of tracks:

    * normal tracks return a single url
    * All Access tracks return multiple urls, which must be combined

    \param song_id  a single song id.

    While acquiring the urls requires authentication, retreiving the
    contents does not.

    However, there are limitations on how the stream urls can be used:

    * the urls expire after a minute
    * only one IP can be streaming music at once. Other attempts will get an http 403 with
    `X-Rejected-Reason: ANOTHER_STREAM_BEING_PLAYED`.

    __This is only intended for streaming__. The streamed audio does not contain metadata.
    Use get_song_download_info() or Musicmanager::download_song() to download files with metadata.
    */
    std::vector<std::string> get_stream_urls(const identifier& song_id);

    /*!
    Equivalent to the 'Fix Incorrect Match' button, this requests re-uploading of songs.
    Returns the song_ids provided.

    \param song_ids a list of song ids to report.

    Note that if you uploaded a song through gmusicapi, it won't be reuploaded
    automatically - this currently only works for songs uploaded with the Music Manager.
    See issue [#89](https://github.com/simon-weber/gmusicapi/issues/89).

    This should only be used on matched tracks.
    */
    identifiers report_incorrect_match(const identifiers& song_ids);

    /*!
    Uploads an image and sets it as the album art for songs.
    Returns a url to the image on Google's servers.

    \param song_ids         a list of song ids, or a single song id.
    \param image_filepath   filepath of the art to use. jpg and png are known to work.

    This function will __always__ upload the provided image, even if it's already uploaded.
    If the art is already uploaded and set for another song, copy over the
    value of the `'albumArtUrl'` key using Mobileclient::change_song_metadata() instead.
    */
    std::string upload_album_art(const identifiers& song_ids, const std::string& image_filepath);

    /*!
    Returns a dictionary with four keys: author, description, num_tracks, and title.

    \param share_token  from `playlist.shareToken`, or a playlist share
                        url (https://play.google.com/music/playlist/<token>).

    Note that tokens from urls will need to be url-decoded,
    eg ``AM...%3D%3D`` becomes ``AM...==``.
    */
    SharedPlaylistInfo get_shared_playlist_info(const std::string& share_token);
};


} // namespace GMusicApi
