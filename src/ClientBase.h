#pragma once
#include "GMusicApi.h"

namespace GMusicApi
{

/*!
Factors out common client setup.
*/
class ClientBase
{
public:
    /*!
    Constructor.

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
    ClientBase(const std::string& client_name, bool debug_logging = true, bool validate = true, bool verify_ssl = true);

    /*!
    get server response validation status
    */
    bool validate() const;

    /*!
    get server response validation status

    \param val  new validation status
    */
    void set_validate(bool val);

protected:
	template<typename Ret, typename ...Args>
	Ret callMethod(const std::string& methodName, Args&&... args) const;

    template<typename T = boost::python::object>
    T getMember(const std::string& memberName) const;
    
    template <typename T>
    void setMember(const std::string& memberName, const T& value);

private:
	GMusicApi& instance() const;
	boost::python::object m_object;
};

template<typename Ret, typename ...Args>
inline Ret ClientBase::callMethod(const std::string& methodName, Args&& ...args) const
{
    namespace bp = boost::python;
    try
    {
        return bp::call_method<Ret>(m_object.ptr(), methodName.c_str(), std::forward<Args>(args)...);
    }
    catch (const bp::error_already_set&)
    {
        handlePythonException();
    }
}

template<typename T>
inline T ClientBase::getMember(const std::string & memberName) const
{
    namespace bp = boost::python;
    try
    {
        return bp::extract<T>(m_object.attr(memberName.c_str()));
    }
    catch (const bp::error_already_set&)
    {
        handlePythonException();
    }
}

template<typename T>
inline void ClientBase::setMember(const std::string & memberName, const T & value)
{
    namespace bp = boost::python;
    try
    {
        m_object.attr(memberName.c_str()) = value;
    }
    catch (const bp::error_already_set&)
    {
        handlePythonException();
    }
}

} // namespace GMusicApi
