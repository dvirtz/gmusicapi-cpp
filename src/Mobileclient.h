#pragma once
#include "ClientBase.h"
#include "Song.h"
#include "SongQuality.h"
#include "RegisteredDevice.h"
#include <string>
#include <vector>
#include <boost/date_time.hpp>

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
	\param	validate	if `false`, do not validate server responses against known schemas. 
						This helps to catch protocol changes, but requires significant cpu work.
						This arg is stored and can be safely modified at runtime by calling validate().
	\param	verify_ssl  if `false`, exceptions will not be raised if there are problems verifying SSL certificates. 
						Be wary of using this option; it's almost always better to fix the machine's SSL 
						configuration than to ignore errors.
	*/
	Mobileclient(bool debug_logging = true, bool validate = true, bool verify_ssl = true);

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
			   const std::string& android_id = std::string()) const;

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
	std::string get_stream_url(const std::string& song_id,
							   const std::string& device_id = std::string(),
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
	std::vector<std::string> change_song_metadata(const std::vector<Song>& songs);

    /*!
    Deletes songs from the library.

    \param song_ids     a list of song ids, or a single song id.
    \return a list of deleted song ids.
    */
    std::vector<std::string> delete_songs(const std::vector<std::string> song_ids);

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
    \param plays    (optional) positive number of plays to increment by. The default is 1.
    \param playtime (optional) a time the song was played. It will default to the time of the call.
     */
    std::string increment_song_playcount(const std::string& song_id,
                                         int plays = 1,
                                         const boost::posix_time::ptime& playtime = boost::posix_time::microsec_clock::local_time());
};


} // namespace GMusicApi
