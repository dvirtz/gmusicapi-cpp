#pragma once
#include "ModuleBase.h"

namespace PythonHelper
{

template<typename Module>
class ClassBase
{
public:
    template<typename... Args>
    ClassBase(const std::string& name, Args&&... args);
    
    virtual ~ClassBase() = default;

protected:

    template<typename Ret, typename ...Args>
    static Ret callStaticMethod(const std::string & className,
                                const std::string& methodName, 
                                Args&&... args);

    template<typename Ret, typename ...Args>
    Ret callMethod(const std::string& methodName, Args&&... args) const;

    template<typename T = boost::python::object>
    T getMember(const std::string& memberName) const;

    template <typename T>
    void setMember(const std::string& memberName, const T& value);

    static Module& module();

    std::string             m_name;
    boost::python::object   m_object;
};

template<typename Module>
template<typename ...Args>
inline ClassBase<Module>::ClassBase(const std::string & name, Args && ...args)
    : m_name(name), m_object(module().createObject(name, std::forward<Args>(args)...))
{}

template<typename Module>
template<typename Ret, typename ...Args>
inline Ret ClassBase<Module>::callStaticMethod(const std::string & className, 
                                               const std::string & methodName, 
                                               Args && ...args)
{
    return module().callStaticMethod<Ret>(className, methodName, std::forward<Args>(args)...);
}

template<typename Module>
template<typename Ret, typename ...Args>
inline Ret ClassBase<Module>::callMethod(const std::string& methodName, Args&& ...args) const
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

template<typename Module>
template<typename T>
inline T ClassBase<Module>::getMember(const std::string & memberName) const
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

template<typename Module>
template<typename T>
inline void ClassBase<Module>::setMember(const std::string & memberName, const T & value)
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

template<typename Module>
inline Module & ClassBase<Module>::module()
{
    return Module::instance();
}

} // namespace PythonHelper
