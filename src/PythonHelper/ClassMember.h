#pragma once

/*!
define PythonHelper::ClassBase member variable
\param type     the member type
\param name     the member name
\param parent   type of class holding the member variable
*/
#define CLASS_MEMBER(type, name, parent) \
    struct name##_s \
    { \
        name##_s(parent & p) : m_p(p) {} \
        parent & m_p; \
        operator type () const\
        { \
            return m_p.getMember<type>(#name); \
        } \
        name##_s operator=(const type & val) \
        { \
            m_p.setMember(#name, val); \
            return *this; \
        } \
    }; \
    name##_s name{*this}
