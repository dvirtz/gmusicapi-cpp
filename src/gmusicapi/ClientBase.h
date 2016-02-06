#pragma once
#include "ClassBase.h"
#include "ClassMember.h"

namespace GMusicApi
{

class Module;

/*!
Factors out common client setup.
*/
class ClientBase : public PythonHelper::ClassBase
{
public:
    /*!
    Constructor.

    \param  module          loaded GMusicApi module
    \param  client_name     either Mobileclient, Webclient or Musicmanager
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
    ClientBase(Module& module,
               const std::string& client_name, 
               bool debug_logging = true, 
               bool validate = true, 
               bool verify_ssl = true);

    /*!
    get server response validation status
    */
    bool validate();

    void setValidate(bool val);
};

} // namespace GMusicApi
