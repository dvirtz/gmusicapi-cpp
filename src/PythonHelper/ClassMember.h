#pragma once

namespace PythonHelper
{

template<typename T, typename Parent>
struct ClassMember
{
    ClassMember(Parent& p, const std::string& name) : m_p(p), m_name(name) 
    {
        // register converter to python
        static bool registerTypeConverter = []()
        {
            namespace bp = boost::python;
            struct ClassMemberToPython
            {
                static PyObject* convert(const ClassMember& cm)
                {
                    return bp::incref(bp::converter::arg_to_python<T>(static_cast<T>(cm)).get());
                }
            };
            bp::to_python_converter<ClassMember, ClassMemberToPython>();
            return true;
        }();
    }

    operator T() const
    {
        return m_p.template getMember<T>(m_name);
    }

    ClassMember& operator=(const T& val)
    {
        m_p.setMember(m_name, val);
        return *this;
    }

private:
    Parent& m_p;
    std::string m_name;
};

/*!
define member variable
\param type     the member type
\param name     the member name
\param parent   type of class holding the member variable
*/
#define CLASS_MEMBER(type, name, parent) \
    friend struct PythonHelper::ClassMember<type, parent>; \
    PythonHelper::ClassMember<type, parent> name{*this, #name}

/*!
define const member variable
\param type     the member type
\param name     the member name
\param parent   type of class holding the member variable
*/
#define CONST_CLASS_MEMBER(type, name, parent) \
    friend struct PythonHelper::ClassMember<type, parent>; \
    const PythonHelper::ClassMember<type, parent> name{*this, #name}

} // namespace PythonHelper
