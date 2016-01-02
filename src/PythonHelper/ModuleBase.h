#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/optional.hpp>
#include <datetime.h>
MSC_RESTORE_WARNINGS
#include <string>
#include <mutex>

namespace PythonHelper
{

template<typename Derived>
class ModuleBase
{
public:
    static Derived& instance();

    virtual ~ModuleBase();

    template<typename... Args>
    boost::python::object createObject(const std::string& name, Args&&... args) const;

    template<typename Ret, typename... Args>
    Ret callStaticMethod(const std::string& className, 
                         const std::string& methodName,
                         Args&&... args) const;

protected:
    ModuleBase(const std::string& name, const boost::optional<std::string>& path = {});
    ModuleBase(const ModuleBase&) = delete;
    ModuleBase& operator=(const ModuleBase&) = delete;

protected:
	boost::python::object m_dict;
};

template<typename Derived>
inline Derived& ModuleBase<Derived>::instance()
{
    static Derived theInstance;
    return theInstance;
}

template<typename Derived>
template<typename ...Args>
inline boost::python::object ModuleBase<Derived>::createObject(const std::string & name, Args&& ...args) const
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

template<typename Derived>
template<typename Ret, typename... Args>
inline Ret ModuleBase<Derived>::callStaticMethod(const std::string& className,
                                                 const std::string& methodName,
                                                 Args&&... args) const
{
    namespace bp = boost::python;
    try
    {
        auto classObject = m_dict[className];
        bp::object methodObject = classObject.attr("__dict__")[methodName];
        return bp::call<Ret>(methodObject.ptr(), std::forward<Args>(args)...);
    }
    catch (const bp::error_already_set&)
    {
        handlePythonException();
    }
}

template<typename Derived>
ModuleBase<Derived>::ModuleBase(const std::string & name, const boost::optional<std::string>& path)
{
    namespace bp = boost::python;

    Py_Initialize();
    PyDateTime_IMPORT;

    try
    {
        if (path)
        {
            // add module to path
            auto sys = bp::import("sys");
            bp::list pythonPath = bp::extract<bp::list>(sys.attr("path"));
            pythonPath.append(*path);
        }

        // import gmusicapi module
        auto module = bp::import(name.c_str());
        m_dict = module.attr("__dict__");
    }
    catch (const bp::error_already_set&)
    {
        handlePythonException();
    }
}

template<typename Derived>
ModuleBase<Derived>::~ModuleBase() = default;
	
} // namespace PythonHelper
