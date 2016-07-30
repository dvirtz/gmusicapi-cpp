#pragma once

#include "gmusicapi/ClientBase.h"
#include "gmusicapi/genDefs.h"
#include "gmusicapi/Clients.h"
#include "gmusicapi/Song.h"
#include <boost/variant.hpp>

namespace GMusicApi
{

/*!
Allows uploading by posing as Google’s Music Manager.

Musicmanager uses OAuth, so a plaintext email and password are not required when logging in.

For most authors and users of gmusicapi scripts, perform_oauth() should be run once per machine
to store credentials to disk. Future calls to login() can use use the stored credentials by default.

Some authors may want more control over the OAuth flow.
In this case, credentials can be directly provided to login().
*/
class Musicmanager : public ClientBase
{
public:
    /*!
    Constructor.

    \param  module          loaded GMusicApi module
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
    Musicmanager(Module& module,
                 bool debug_logging = true,
                 bool validate = true,
                 bool verify_ssl = true);

    /*!
    Provides a series of prompts for a user to follow to authenticate.

    In most cases, this should only be run once per machine to store
    credentials to disk, then never be needed again.

    If the user refuses to give access,
    `oauth2client.client.FlowExchangeError` is raised.

    \param storage_filepath a filepath to write the credentials to.
                            if not given, the credentials won't be written to disk
                            (which is not recommended).

    \param open_browser     if `true`, attempt to open the auth url in the system
                            default web browser.
                            The url will be printed regardless of this param's setting.
    */
    void perform_oauth(const boost::optional<std::string>& storage_filepath = {},
                       bool open_browser = false);

    /*!
    Authenticates the Music Manager using OAuth.
    Returns `true` on success, `false` on failure.

    Unlike the Webclient, OAuth allows authentication without
    providing plaintext credentials to the application.

    In most cases, the default parameters should be acceptable. Users on
    virtual machines will want to provide `uploader_id`.

    \param oauth_credentials    a path to OAuth storage file.
                                By default, the same default path used by perform_oauth() is used.
                                Endusers will likely call perform_oauth() once to write credentials
                                to disk and then ignore this parameter.

    \param uploader_id          a unique id as a MAC address, e.g. `'00:11:22:33:AA:BB'`.
                                This should only be provided in cases where the default
                                (host MAC address incremented by 1) will not work.
                                Upload behavior is undefined if a Music Manager uses the same id, especially when
                                reporting bad matches.
                                `ValueError` will be raised if this is provided but not in the proper form.
                                `OSError` will be raised if this is not provided and a real MAC could not be
                                determined (most common when running on a VPS).
                                If provided, use the same id on all future runs for this machine,
                                because of the upload device limit explained below.

    \param uploader_name        human-readable non-unique id; default is `"<hostname> (gmusicapi-{version})"`.
                                This doesn't appear to be a part of authentication at all.
                                Registering with (id, name = X, Y) and logging in with (id, name = X, Z)
                                works, and does not change the server-stored uploader_name.

    There are hard limits on how many upload devices can be registered; refer to
    [Google's docs](http://support.google.com/googleplay/bin/answer.py?hl=en&answer=1230356).
    There have been limits on deauthorizing devices in the past, so it's smart not to register
    more devices than necessary.
    */
    bool login(const boost::optional<std::string>& oauth_credentials = {},
               const boost::optional<identifier>& uploader_id = {},
               const boost::optional<std::string>& uploader_name = {});
    /*!
    Authenticates the Music Manager using OAuth.
    Returns `true` on success, `false` on failure.

    Unlike the Webclient, OAuth allows authentication without
    providing plaintext credentials to the application.

    In most cases, the default parameters should be acceptable. Users on
    virtual machines will want to provide `uploader_id`.

    \param oauth_credentials    an OAuth2Credentials instance;
                                This param is mostly intended to allow flexibility for developers of a
                                3rd party service who intend to perform their own OAuth flow
                                (e.g. on their website).

    \param uploader_id          a unique id as a MAC address, e.g. `'00:11:22:33:AA:BB'`.
                                This should only be provided in cases where the default
                                (host MAC address incremented by 1) will not work.
                                Upload behavior is undefined if a Music Manager uses the same id, especially when
                                reporting bad matches.
                                `ValueError` will be raised if this is provided but not in the proper form.
                                `OSError` will be raised if this is not provided and a real MAC could not be
                                determined (most common when running on a VPS).
                                If provided, use the same id on all future runs for this machine,
                                because of the upload device limit explained below.

    \param uploader_name        human-readable non-unique id; default is `"<hostname> (gmusicapi-{version})"`.
                                This doesn't appear to be a part of authentication at all.
                                Registering with (id, name = X, Y) and logging in with (id, name = X, Z)
                                works, and does not change the server-stored uploader_name.

    There are hard limits on how many upload devices can be registered; refer to
    [Google's docs](http://support.google.com/googleplay/bin/answer.py?hl=en&answer=1230356).
    There have been limits on deauthorizing devices in the past, so it's smart not to register
    more devices than necessary.
    */
    bool login(const OAuth2Credentials& oauth_credentials,
               const boost::optional<identifier>& uploader_id = {},
               const boost::optional<std::string>& uploader_name = {});

    /*!
    Forgets local authentication in this Client instance.

    \param revoke_oauth if `true`, oauth credentials will be permanently revoked.
                        If credentials came from a file, it will be deleted.

        Returns ``True`` on success.
    */
    bool logout(bool revoke_oauth = false);

    /*!
    Uploads the given filepaths.

    All non-mp3 files will be transcoded before being uploaded.
    This is a limitation of Google's backend.

    An available installation of ffmpeg or avconv is required in most cases:
    see [the installation page](https://unofficial-google-music-api.readthedocs.org/en/latest/usage.html?#installation)
    for details.

    Returns a 3-tuple `(uploaded, matched, not_uploaded)` of maps, eg::

        {
            {'<filepath>', '<new server id>'},               # uploaded
            {'<filepath>', '<new server id>'},               # matched
            {'<filepath>', '<reason, eg ALREADY_EXISTS>'}    # not uploaded
        }

    \param filepaths            a list of filepaths, or a single filepath.
    \param transcode_quality    if int, pass to ffmpeg/avconv `-q:a` for libmp3lame
                                ([lower-better int](http://trac.ffmpeg.org/wiki/Encoding%20VBR%20%28Variable%20Bit%20Rate%29%20mp3%20audio)).
                                If string, pass to ffmpeg/avconv `-b:a` (eg `'128k'` for an average bitrate of 128k).
                                The default is 320kbps cbr (the highest possible quality).
    \param enable_matching      if `true`, attempt to use [scan and match]
                                (http://support.google.com/googleplay/bin/answer.py?hl=en&answer=2920799&topic=2450455)
                                to avoid uploading every song.
                                This requires ffmpeg or avconv.
                                __WARNING__: currently, mismatched songs can __not__ be fixed with the 'Fix Incorrect Match'
                                button nor Webclient::report_incorrect_match().
                                They would have to be deleted and reuploaded with matching disabled
                                (or with the Music Manager).
                                Fixing matches from gmusicapi may be supported in a future release; see issue [#89]
                                (https://github.com/simon-weber/gmusicapi/issues/89).

    All Google-supported filetypes are supported; see [Google's documentation]
    (http://support.google.com/googleplay/bin/answer.py?hl=en&answer=1100462).

    Unlike Google's Music Manager, this function will currently allow the same song to
    be uploaded more than once if its tags are changed. This is subject to change in the future.

    If `PERMANENT_ERROR` is given as a not_uploaded reason, attempts to reupload will never
    succeed. The file will need to be changed before the server will reconsider it; the easiest
    way is to change metadata tags (it's not important that the tag be uploaded, just that the
    contents of the file change somehow).
    */
    UploadResults upload(const std::vector<std::string>& filepaths,
                         const boost::variant<int, std::string>& transcode_quality = "320k",
                         bool enable_matching = false);

    /*!
    All Access tracks that were added to the library will not be included,
    only tracks uploaded/matched by the user.

    \param incremental  if `true`, return a generator that yields lists
                        of at most 1000 songs as they are retrieved from the server.
                        This can be useful for presenting a loading bar to a user.
    */
    SongRange get_uploaded_songs(bool incremental = false);

    /*!
    Returns a pair `{'suggested_filename', 'audio_bytestring'}`.
    The filename will be what the Music Manager would save the file as,
    presented as a unicode string with the proper file extension.
    You don't have to use it if you don't want.

    \param song_id  a single song id.

    To write the song to disk, use something like:

        std::tie(filename, audio) = mm.download_song(an_id);

        # if open() throws a UnicodeEncodeError, either use
        #   filename.encode('utf-8')
        # or change your default encoding to something sane =)
        std::ofstream file(filename, std::ios::out | std::ios::binary);
        file.write(audio.data(), audio.size());

    Unlike with Webclient::get_song_download_info() there is no download limit
    when using this interface.

    Also unlike the Webclient, downloading a track requires authentication.
    Returning a url does not suffice, since retrieving a track without auth
    will produce an http 500.
    */
    DownloadResult download_song(const identifier& song_id);
};

} // namespace GMusicApi
