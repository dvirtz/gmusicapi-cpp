#pragma once
#include "GMusicApiUser.h"
#include "Song.h"
#include "SongQuality.h"
#include "RegisteredDevice.h"
#include <string>
#include <vector>

namespace GMusicApi
{

/*Allows library management and streaming by posing as the googleapis.com mobile clients.

Uploading is not supported by this client (use <Musicmanager> to upload). 
*/
class Mobileclient : private GMusicApiUser
{
public:

	/**
	 Constructor.
	
	 \param	debug_logging	each Client has a logger member. 
							The logger is named gmusicapi.<client class><client number> and will propogate to the gmusicapi root logger.
							If this param is true, handlers will be configured to send this client’s debug log output to disk, 
							with warnings and above printed to stderr. Appdirs user_log_dir is used by default. 
							Users can run:
	 
							from gmusicapi.utils import utils
							print utils.log_filepath
							to see the exact location on their system.
	 
							If false, no handlers will be configured; users must create their own handlers.
	 
							Completely ignoring logging is dangerous and not recommended. 
							The Google Music protocol can change at any time; 
							if something were to go wrong, the logs would be necessary for recovery.

	 \param	validate	 	if false, do not validate server responses against known schemas. 
							This helps to catch protocol changes, but requires significant cpu work.

							This arg is stored as self.validate and can be safely modified at runtime.
	 \param	verify_ssl   	if false, exceptions will not be raised if there are problems verifying SSL certificates. 
							Be wary of using this option; it’s almost always better to fix the machine’s SSL 
							configuration than to ignore errors.
	 */
	Mobileclient(bool debug_logging = true, bool validate = true, bool verify_ssl = true);


	/** Destructor. */
	~Mobileclient();

	/**
	 Authenticates the Mobileclient.
	
	 \param	email	  	eg ``'test@gmail.com'`` or just ``'test'``.
	 \param	password  	the account's password. This is not stored locally, and is sent securely over
	 					SSL. App-specific passwords are not supported.
	 \param	android_id	16 hex digits, eg ``'1234567890abcdef'``.
	 					
	 					Pass Mobileclient.FROM_MAC_ADDRESS instead to attempt to use this machine's
	 					MAC address as an android id.
	 					*Use this at your own risk*:
	 					the id will be a non-standard 12 characters, but appears to work fine in
	 					testing. If a valid MAC address cannot be determined on this machine (which
	 					is often the case when running on a VPS), raise OSError.
	
	 \return	true on success, false on failure.
	 */

	bool login(const std::string& email,
			   const std::string& password, 
			   const std::string& android_id = std::string()) const;

	/**
	 Forgets local authentication in this Api instance.
	
	 \return	true on success.
	 */

	bool logout() const;

	/**
	 Returns a list of dictionaries that each represent a song.
	
	 \param	incremental	   	if true, return a generator that yields lists of at most 1000 tracks as
	 						they are retrieved from the server. This can be useful for presenting a
	 						loading bar to a user.
	 						
	 \param	include_deleted	if true, include tracks that have been deleted in the
	 						past.
	
	 \return	all songs.
	 */

	SongRange get_all_songs(bool incremental = false, bool include_deleted = false);

	/**
	 Returns a url that will point to an mp3 file.
	
	 \param	song_id  	a single song id.
	 \param	device_id	defaults to android_id from login. Otherwise, provide a mobile device id as a
	 					string. Android device ids are 16 characters, while iOS ids are uuids with
	 					‘ios:’ prepended.
	 					
	 					If you have already used Google Music on a mobile device,
	 					Mobileclient::get_registered_devices will provide at least one working id.
	 					Omit '0x' from the start of the string if present.
	 					
	 					Registered computer ids (a MAC address) will not be accepted and will 403.
	 					
	 					Providing an unregistered mobile device id will register it to your account,
	 					subject to Google’s device limits
	 					http://support.google.com/googleplay/bin/answer.py?hl=en&amp;answer=1230356.
	 					*Registering a device id that you do not own is likely a violation of the
	 					TOS.* Registering a device id that you do not own is likely a violation of
	 					the TOS.
	 \param	quality  	stream bits per second quality.

	 When handling the resulting url, keep in mind that:
	 * you will likely need to handle redirects
	 * the url expires after a minute
	 * only one IP can be streaming music at once.
	 This can result in an http 403 with
	 ``X-Rejected-Reason: ANOTHER_STREAM_BEING_PLAYED``.

	 The file will not contain metadata.
	 Use Webclient::get_song_download_info or Musicmanager::download_song to download files with metadata.
	
	 \return	The stream URL.
	 */

	std::string get_stream_url(const std::string& song_id,
							   const std::string& device_id = std::string(),
							   SongQuality quality = SongQuality::High);

	std::vector<RegisteredDevice> get_registered_devices();
};


} // namespace GMusicApi
