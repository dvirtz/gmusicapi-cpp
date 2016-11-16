#pragma once

#include "PythonHelper/utility.h"
MSC_DISABLE_WARNINGS
#include <pybind11/pybind11.h>
#include <boost/optional.hpp>
MSC_RESTORE_WARNINGS
#include <string>

namespace PythonHelper
{

class ModuleBase
{
public:
    ModuleBase(const char* name, const boost::optional<std::string>& path = {});
    virtual ~ModuleBase();

    template<typename... Args>
    pybind11::object createObject(const char* name, Args&&... args) const;

    template<typename Ret, typename... Args>
    Ret callStaticMethod(const char* className,
                         const char* methodName,
                         Args&&... args) const;

    template<typename Ret, typename... Args>
    Ret callGlobalMethod(const char* methodName,
                         Args&&... args) const;

protected:

    template<typename T = pybind11::object>
    T getMember(const char* memberName) const;

    template <typename T>
    void setMember(const char* memberName, const T& value);

protected:
    pybind11::dict m_dict;
};

template<typename ...Args>
inline pybind11::object ModuleBase::createObject(const char* name, Args&& ...args) const
{
    namespace py = pybind11;
    py::function f {m_dict[name]};
    return f(std::forward<Args>(args)...);
}

template<typename Ret, typename... Args>
inline Ret ModuleBase::callStaticMethod(const char* className,
                                        const char* methodName,
                                        Args&&... args) const
{
    namespace py = pybind11;
    auto classObject = m_dict[className];
    py::object methodObject {classObject.attr("__dict__")[methodName]};
    py::function funcObject {methodObject.attr("__func__")};
    return funcObject(std::forward<Args>(args)...);
}

template<typename T>
inline T ModuleBase::getMember(const char* memberName) const
{
    namespace py = pybind11;
    return m_dict[memberName].cast<T>();
}

template<typename T>
inline void ModuleBase::setMember(const char* memberName, const T & value)
{
    namespace py = pybind11;
    m_dict[memberName] = py::cast(value);
}

template<typename Ret, typename ...Args>
inline Ret ModuleBase::callGlobalMethod(const char* methodName, Args && ...args) const
{
    namespace py = pybind11;
    py::function methodObject {m_dict[methodName]};
    auto ret = methodObject(std::forward<Args>(args)...);
    return ret.template cast<Ret>();
}

} // namespace PythonHelper
