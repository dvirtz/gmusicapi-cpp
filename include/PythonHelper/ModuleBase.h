#pragma once

#include "PythonHelper/utility.h"
MSC_DISABLE_WARNINGS
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/optional.hpp>
MSC_RESTORE_WARNINGS
#include "PythonHelper/Initializer.h"
#include <string>

namespace PythonHelper
{

class ModuleBase
{
public:
    ModuleBase(const std::string& name, const boost::optional<std::string>& path = {});
    virtual ~ModuleBase();

    template<typename... Args>
    boost::python::object createObject(const std::string& name, Args&&... args) const;

    template<typename Ret, typename... Args>
    Ret callStaticMethod(const std::string& className, 
                         const std::string& methodName,
                         Args&&... args) const;

    template<typename Ret, typename... Args>
    Ret callGlobalMethod(const std::string& methodName,
                         Args&&... args) const;

protected:
    
    template<typename T = boost::python::object>
    T getMember(const std::string& memberName) const;

    template <typename T>
    void setMember(const std::string& memberName, const T& value);

protected:
	boost::python::object m_dict;
};

template<typename ...Args>
inline boost::python::object ModuleBase::createObject(const std::string & name, Args&& ...args) const
{
    namespace bp = boost::python;
    try
    {
        bp::object type = m_dict[name];
        return bp::call<boost::python::object>(type.ptr(), std::forward<Args>(args)...);
    }
    catch (const bp::error_already_set&)
    {
        handlePythonException();
    }
}

template<typename Ret, typename... Args>
inline Ret ModuleBase::callStaticMethod(const std::string& className,
                                                 const std::string& methodName,
                                                 Args&&... args) const
{
    namespace bp = boost::python;
    try
    {
        auto classObject = m_dict[className];
        bp::object methodObject = classObject.attr("__dict__")[methodName];
        bp::object funcObject = methodObject.attr("__func__");
        return bp::call<Ret>(funcObject.ptr(), std::forward<Args>(args)...);
    }
    catch (const bp::error_already_set&)
    {
        handlePythonException();
    }
}

template<typename T>
inline T ModuleBase::getMember(const std::string & memberName) const
{
    namespace bp = boost::python;
    try
    {
        return bp::extract<T>(m_dict[memberName]);
    }
    catch (const bp::error_already_set&)
    {
        handlePythonException();
    }
}

template<typename T>
inline void ModuleBase::setMember(const std::string & memberName, const T & value)
{
    namespace bp = boost::python;
    try
    {
        m_dict[memberName] = value;
    }
    catch (const bp::error_already_set&)
    {
        handlePythonException();
    }
}

template<typename Ret, typename ...Args>
inline Ret ModuleBase::callGlobalMethod(const std::string & methodName, Args && ...args) const
{
    namespace bp = boost::python;
    try
    {
        bp::object methodObject = m_dict[methodName];
        return bp::call<Ret>(methodObject.ptr(), std::forward<Args>(args)...);
    }
    catch (const bp::error_already_set&)
    {
        handlePythonException();
    }
}

} // namespace PythonHelper
