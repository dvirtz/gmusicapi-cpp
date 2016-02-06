#pragma once
#include "ClientBase.h"
#include "Song.h"
#include "SongQuality.h"
#include "RegisteredDevice.h"
#include "Playlist.h"
#include "RadioStation.h"
#include "SearchResults.h"
#include "Genre.h"
#include <string>
#include <vector>
#include <boost/date_time.hpp>
#include <boost/optional.hpp>

namespace GMusicApi
{

/*!
Allows library management and streaming by posing as the googleapis.com mobile clients.

Uploading is not supported by this client (use Musicmanager to upload).
*/
class Mobileclient : public ClientBase
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
    Mobileclient(Module& module,
                 bool debug_logging = true,
                 bool validate = true, 
                 bool verify_ssl = true);

    /*!
    Authenticates the Mobileclient.

    \param	email	  	eg `'test@gmail.com'` or just `'test'`.
    \param	password  	the account's password. This is not stored locally, and is sent securely over
                        SSL. App-specific passwords are not supported.
    \param	android_id	16 hex digits, eg `'1234567890abcdef'`.
                        pass an empty string instead to attempt to use this machine's
                        MAC address as an android id.
                        __Use this at your own risk:__
                        the id will be a non-standard 12 characters, but appears to work fine in
                        testing. If a valid MAC address cannot be determined on this machine (which
                        is often the case when running on a VPS), raise OSError.

    \return	`true` on success, `false` on failure.
    */
    bool login(const std::string& email,
               const std::string& password,
               const identifier& android_id = {}) const;

    /*!
    Forgets local authentication in this instance.

    \return	`true` on success, `false` on failure.
    */
    bool logout() const;

    /*!
    Returns a range of songs.

    \param	incremental	    if `true`, returns a lazily evaluated range that yields lists of at most 1000 tracks as
                        they are retrieved from the server. This can be useful for presenting a
                        loading bar to a user.

    \param	include_deleted	if `true, include tracks that have been deleted in the past.
     */
    SongRange get_all_songs(bool incremental = false, bool include_deleted = false);

    /*!
    Returns a url that will point to an mp3 file.

    \param	song_id  	a single song id.
    \param	device_id	defaults to `android_id` from login. Otherwise, provide a mobile device
                        id as a string. Android device ids are 16 characters, while iOS ids are
                        uuids with `ios:` prepended.
                        If you have already used Google Music on a mobile device,
                        get_registered_devices() will provide at least one working id.
                        Omit `0x` from the start of the string if present.
                        Registered computer ids (a MAC address) will not be accepted and will 403.
                        Providing an unregistered mobile device id will register it to your account,
                        subject to Google's [device limits](http://support.google.com/googleplay/bin/answer.py?hl=en&amp;answer=1230356).
                        __Registering a device id that you do not own is likely a violation of the
                        TOS.__
    \param	quality  	stream bits per second quality.

    When handling the resulting url, keep in mind that:
    - you will likely need to handle redirects
    - the url expires after a minute
    - only one IP can be streaming music at once. This can result in an http 403 with
    `X-Rejected-Reason: ANOTHER_STREAM_BEING_PLAYED`.

    The file will not contain metadata. Use Webclient::get_song_download_info or Musicmanager::download_song
    to download files with metadata.
    */
    std::string get_stream_url(const identifier& song_id,
                               const boost::optional<identifier>& device_id = {},
                               SongQuality quality = SongQuality::High);

    /*!
    Returns a list of dictionaries representing devices associated with the account.

    Performing the Musicmanager OAuth flow will register a device
    of type `'DESKTOP_APP'`.

    Installing the Android or iOS Google Music app and logging into it will
    register a device of type `'ANDROID'` or `'IOS'` respectively, which is
    required for streaming with the Mobileclient.
    */
    std::vector<RegisteredDevice> get_registered_devices();

    /*!
    Changes the metadata of tracks.

    \param songs    songs whose metadata is to be changed.

    \return a list of the song ids changed

    Currently, only the Song::rating key can be changed.
    Set it to `"0"` (no thumb), `"1"` (down thumb), or `"5"` (up thumb)
    unless you're using the 5-star ratings lab.

    You can also use this to rate All Access tracks
    that aren't in your library, eg::

        auto song = mc.get_track_info('<some store track id>');
        song.rating = "5";
        mc.change_song_metadata({song});
    */
    identifiers change_song_metadata(const std::vector<Song>& songs);

    /*!
    Deletes songs from the library.

    \param song_ids     a list of song ids, or a single song id.
    \return a list of deleted song ids.
    */
    identifiers delete_songs(const identifiers& song_ids);

    /*!
    Returns a list of dictionaries that each represent a track.

    Only All Access tracks will be returned.

    Promoted tracks are determined in an unknown fashion,
    but positively-rated library tracks are common.

    See get_track_info() for the format of a track dictionary.
    */
    SongRange get_promoted_songs();

    /*!
    Increments a song's playcount and returns its song id.

    \param song_id  a song id. Providing the id of an AA track that has been added to the library
                    will __not__ increment the corresponding library song's playcount.
                    To do this, use the 'id' field (which looks like a uuid and doesn't begin with 'T'),
                    not the 'nid' field.
    \param plays    positive number of plays to increment by. The default is 1.
    \param playtime a time the song was played. It will default to the time of the call.
     */
    identifier increment_song_playcount(const identifier& song_id,
                                        int plays = 1,
                                        const boost::optional<boost::posix_time::ptime>& playtime = {});

    /*!
    Returns a range of playlists.

    \param incremental  if `true`, return a generator that yields lists of at most 1000 playlists
                        as they are retrieved from the server. This can be useful for presenting
                        a loading bar to a user.
    \param include_deleted  if `true`, include playlists that have been deleted in the past.
    */
    PlaylistRange get_all_playlists(bool incremental = false, bool include_deleted = false);

    /*!
    Retrieves the contents of __all__ user-created playlists - the Mobileclient does not support retrieving
    only the contents of one playlist.

    This will not return results for public playlists that the user is subscribed to;
    use get_shared_playlist_contents() instead.

    The same structure as get_all_playlists() will be returned, but with the addition of
    the `tracks` field in each Playlist set to a list of properly-ordered tracks.
    */
    PlaylistRange get_all_user_playlist_contents();

    /*!
    Creates a new empty playlist and returns its id.

    \param name         the desired title.
                        Creating multiple playlists with the same name is allowed
    \param description  the desired description
    \param isPublic     if `true`, creates a public All Access playlist.
    */
    identifier create_playlist(const std::string& name,
                               const std::string& description = std::string(),
                               bool isPublic = false);

    /*!
    Deletes a playlist and returns its id.

    \param playlist_id  the id to delete.
    */
    identifier delete_playlist(const identifier& playlist_id);

    /*
    Changes the name of a playlist and returns its id.

    \param playlist_id      the id of the playlist
    \param new_name         desired title
    \param new_description  desired description
    \param isPublic         if `true` and the user has All Access, share playlist.
    */
    identifier edit_playlist(const identifier& playlist_id,
                             const boost::optional<std::string>& new_name = {},
                             const boost::optional<std::string>& new_description = {},
                             const boost::optional<bool>& isPublic = {});

    /*!
    Appends songs to the end of a playlist.
    Returns a list of playlist entry ids that were added.

    \param playlist_id  the id of the playlist to add to.
    \param song_ids     a list of song ids, or a single song id.

    Playlists have a maximum size of 1000 songs.
    Calls may fail before that point (presumably) due to an error on Google's end
    (see [#239](https://github.com/simon-weber/gmusicapi/issues/239>)).
    */
    identifiers add_songs_to_playlist(const identifier& playlist_id, const identifiers& song_ids);

    /*!
    Reorders a single entry in a playlist and returns its id.

    Read `reorder_playlist_entry(foo, bar, gaz)` as "reorder playlist entry __foo__ to follow entry
    __bar__ and precede entry __gaz__."

    \param entry            the playlist entry to move.
    \param to_follow_entry  the playlist entry that will come before `entry` in the resulting playlist.
                            If not set, `entry` will be the first entry in the playlist.
    \param to_precede_entry the playlist entry that will come after `entry` in the resulting playlist.
                            if not set, `entry` will be the last entry in the playlist.

    `reorder_playlist_entry(foo)` is invalid and will raise ValueError;
    provide at least one of `to_follow_entry` or `to_precede_entry`.

    Not setting `to_follow_entry` or `to_precede_entry` when `entry` is not to be the first or last
    entry in the playlist is undefined.

    All params are entries returned by get_all_user_playlist_contents() or get_shared_playlist_contents().
    */
    identifiers reorder_playlist_entry(const PlaylistEntry& entry,
                                       const boost::optional<PlaylistEntry>& to_follow_entry = {},
                                       const boost::optional<PlaylistEntry>& to_precede_entry = {});

    /*!
    Removes specific entries from a playlist.
    Returns a list of entry ids that were removed.

    \param entry_ids    a list of entry ids, or a single entry id.
    */
    identifiers remove_entries_from_playlist(const identifiers& entry_ids);

    /*!
    Retrieves the contents of a public All Access playlist.

    \param share_token  from `Playlist::shareToken`, or a playlist share url
    (https://play.google.com/music/playlist/<token>).

    Note that tokens from urls will need to be url-decoded,
    eg ``AM...%3D%3D`` becomes ``AM...==``.

    For example, to retrieve the contents of a playlist that the user is
    subscribed to::

        auto all_playlists = mc.get_all_playlists();
        std::vector subscribed_to;
        std::copy_if(all_playlists.begin(), all_playlists.end(), std::back_inserter(subscribed_to), [](const Playlist& p){p.type == "shared";});
        auto share_tok = subscribed_to[0].shareToken;
        auto tracks = mc.get_shared_playlist_contents(share_tok);

    The user need not be subscribed to a playlist to list its tracks.

    Returns a list of playlist entries
    with structure the same as those
    returned by get_all_user_playlist_contents(), but with the `clientId` and `playlistId` empty.
    */
    PlaylistRange get_shared_playlist_contents(const std::string& share_token);

    /*!
    Returns a list of dictionaries that each represent a radio station.

    \param incremental      if `true`, return a generator that yields lists of at most 1000 stations
                            as they are retrieved from the server. This can be useful for
                            presenting a loading bar to a user.
    \param include_deleted  if `true`, include stations that have been deleted in the past.
    \param updated_after    a datetime.datetime; defaults to unix epoch
    */
    RadioStationRange get_all_stations(bool incremental = false,
                                       bool include_deleted = false,
                                       const boost::optional<boost::posix_time::ptime>& updated_after = {});

    /*!
    Returns a list of dictionaries that each represent a track.

    Each call performs a separate sampling (with replacement?)
    from all possible tracks for the station.

    Nonexistent stations will return an empty list.

    \param station_id           the id of a radio station to retrieve tracks from.
                                Use the special id `'IFL'` for the "I'm Feeling Lucky" station.
    \param num_tracks           the number of tracks to retrieve
    \param recently_played_ids  a list of recently played track ids retrieved from this station. 
                                This avoids playing duplicates.
    */
    SongRange get_station_tracks(identifier station_id,
                                 size_t num_tracks = 25,
                                 const boost::optional<identifiers>& recently_played_ids = {});

    /*!
    Creates an All Access radio station and returns its id.

    \param name     the name of the station to create
    \param \*_id    the id of an item to seed the station from.
                    Exactly one of these params must be provided, or ValueError
                    will be raised.
    */
    identifier create_station(const std::string& name,
                              const boost::optional<identifier>& track_id = {}, 
                              const boost::optional<identifier>& artist_id = {}, 
                              const boost::optional<identifier>& album_id = {},
                              const boost::optional<identifier>& genre_id = {});

    /*!
    Deletes All Access radio stations and returns their ids.

    \param station_ids  a single id, or a list of ids to delete.
    */
    identifiers delete_stations(const identifiers& station_ids);

    /*!
    Queries the server for All Access songs, albums and shared playlists.

    Using this method without an All Access subscription will always result in
    CallFailure being raised.

    \param query        a string keyword to search with. 
                        Capitalization and punctuation are ignored.
    \param max_results  Maximum number of items to be retrieved
    */
    SearchResults search_all_access(const std::string& query, size_t max_results = 50);

    /*!
    Adds an All Access track to the library, returning the library track id.

    \param aa_song_id   All Access song id
    */
    identifier add_aa_track(const identifier& aa_song_id);

    /*!
    Retrieves details on an artist.

    \param artist_id        an All Access artist id (hint: they always start with 'A')
    \param include_albums   when `true`, fills the `'albums'` field
    \param max_top_tracks   maximum number of top tracks to retrieve
    \param max_rel_artist   maximum number of related artists to retrieve

    Using this method without an All Access subscription will always result in
    CallFailure being raised.
    */
    Artist get_artist_info(const identifier& artist_id,
                           bool include_albums = true,
                           size_t max_top_tracks = 5,
                           size_t max_rel_artist = 5);

    /*!
    Retrieves details on an album.

    \param album_id         an All Access album id (hint: they always start with 'B')
    \param include_tracks   when `true`, fills the `'tracks'` fields

    Using this method without an All Access subscription will always result in
    CallFailure being raised.
    */
    Album get_album_info(const identifier& album_id, bool include_tracks = true);

    /*!
    Retrieves information about a store track.

    \param store_track_id   an All Access track id (hint: they always start with 'T')

    Using this method without an All Access subscription will always result in
    CallFailure being raised.
    */
    Song get_track_info(const identifier& store_track_id);

    /*!
    Retrieves information on Google Music genres.

    \param parent_genre_id  If provided, only child genres will be returned. 
                            By default, all root genres are returned.
                            If this id is invalid, an empty list will be returned.

    Using this method without an All Access subscription will always result in
    CallFailure being raised.

    Note that the id can be used with create_station()
    to seed an All Access radio station.
    */
    GenreRange get_genres(const boost::optional<identifier>& parent_genre_id = {});
};


} // namespace GMusicApi
