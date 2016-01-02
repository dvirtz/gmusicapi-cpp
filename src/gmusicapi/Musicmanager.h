#pragma once

#include "ClientBase.h"

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
    Musicmanager(bool debug_logging = true, 
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
    static void perform_oauth(const boost::optional<std::string>& storage_filepath = {},
                              bool open_browser = false);
};

} // namespace GMusicApi
