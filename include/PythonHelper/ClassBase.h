#pragma once
#include "PythonHelper/ModuleBase.h"

namespace PythonHelper
{

class ClassBase
{
public:
    template<typename... Args>
    ClassBase(ModuleBase& module, const char* name, Args&&... args);

    virtual ~ClassBase() = default;

protected:

    template<typename Ret, typename ...Args>
    Ret callStaticMethod(const char* className,
                         const char* methodName,
                         Args&&... args) const;

    template<typename Ret, typename ...Args>
    Ret callMethod(const char* methodName, Args&&... args) const;

    template<typename T = pybind11::object>
    T getMember(const char* memberName) const;

    template <typename T>
    void setMember(const char* memberName, const T& value);

    ModuleBase&             m_module;
    std::string             m_name;
    pybind11::object   m_object;
};

template<typename ...Args>
inline ClassBase::ClassBase(ModuleBase& module, const char* name, Args && ...args)
    : m_module(module), m_name(name), m_object(module.createObject(name, std::forward<Args>(args)...))
{}

template<typename Ret, typename ...Args>
inline Ret ClassBase::callStaticMethod(const char* className,
                                       const char* methodName,
                                       Args && ...args) const
{
    return m_module.callStaticMethod<Ret>(className, methodName, std::forward<Args>(args)...);
}

template<typename Ret, typename ...Args>
inline Ret ClassBase::callMethod(const char* methodName, Args&& ...args) const
{
    namespace py = pybind11;
    py::function f {m_object.attr(methodName)};
    return f(std::forward<Args>(args)...).template cast<Ret>();
}

template<typename T>
inline T ClassBase::getMember(const char* memberName) const
{
    namespace py = pybind11;
    return m_object.attr(memberName).template cast<T>();
}

template<typename T>
inline void ClassBase::setMember(const char* memberName, const T & value)
{
    namespace py = pybind11;
    m_object.attr(memberName) = py::cast(value);
}

} // namespace PythonHelper
