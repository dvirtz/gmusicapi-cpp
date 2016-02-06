#pragma once
#include "ModuleBase.h"

namespace PythonHelper
{

class ClassBase
{
public:
    template<typename... Args>
    ClassBase(ModuleBase& module, const std::string& name, Args&&... args);

    virtual ~ClassBase() = default;

protected:

    template<typename Ret, typename ...Args>
    Ret callStaticMethod(const std::string & className,
                         const std::string& methodName,
                         Args&&... args) const;

    template<typename Ret, typename ...Args>
    Ret callMethod(const std::string& methodName, Args&&... args) const;

    template<typename T = boost::python::object>
    T getMember(const std::string& memberName) const;

    template <typename T>
    void setMember(const std::string& memberName, const T& value);

    ModuleBase&             m_module;
    std::string             m_name;
    boost::python::object   m_object;
};

template<typename ...Args>
inline ClassBase::ClassBase(ModuleBase& module, const std::string & name, Args && ...args)
    : m_module(module), m_name(name), m_object(module.createObject(name, std::forward<Args>(args)...))
{}

template<typename Ret, typename ...Args>
inline Ret ClassBase::callStaticMethod(const std::string & className,
                                       const std::string & methodName,
                                       Args && ...args) const
{
    return m_module.callStaticMethod<Ret>(className, methodName, std::forward<Args>(args)...);
}

template<typename Ret, typename ...Args>
inline Ret ClassBase::callMethod(const std::string& methodName, Args&& ...args) const
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
inline T ClassBase::getMember(const std::string & memberName) const
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
inline void ClassBase::setMember(const std::string & memberName, const T & value)
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

} // namespace PythonHelper
