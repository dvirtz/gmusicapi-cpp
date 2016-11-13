#pragma once

#include "PythonHelper/utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/iter_fold.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/mpl/range_c.hpp>
#include <datetime.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/optional.hpp>
#include <boost/variant.hpp>
MSC_RESTORE_WARNINGS
#include <tuple>
#include <type_traits>

// convert python types to user defined C++ types
// based on https://misspent.wordpress.com/2009/09/27/how-to-write-boost-python-converters/

namespace PythonHelper
{

template <typename First, typename F>
inline void
apply(const First& /*first*/, const First& /*last*/, F /*f*/)
{}

template <typename First, typename Last, typename F>
inline void
apply(const First& first, const Last& last, F f)
{
    namespace bf = boost::fusion;
    namespace bfe = bf::extension;

    auto& member = bf::deref(first);
    using seq_type = typename std::remove_const<typename First::seq_type>::type;
    auto name = bfe::struct_member_name<seq_type, First::index::value>::call();
    f(name, member);

    apply(boost::fusion::next(first), last, f);
}

} // namespace PythonHelper

namespace pybind11
{

namespace detail
{

template<typename Struct>
struct type_caster<Struct, typename std::enable_if<boost::fusion::traits::is_sequence<Struct>::value>::type>
{
    PYBIND11_TYPE_CASTER(Struct, _(""));

    // Convert Python->C++
    bool load(handle src, bool /*implicit*/)
    {
        namespace bf = boost::fusion;
        using PythonHelper::getFromDict;

        dict d {src, true};

        if (!d.check())
        {
            return false;
        }

        PythonHelper::apply(bf::begin(value), bf::end(value), [&d, this](const std::string& name, auto& member)
        {
            if (getFromDict(d, name.c_str(), member))
            {
                value.isInitialized.insert(name);
            }
        });

        return true;
    }

    static handle cast(Struct src, return_value_policy /* policy */, handle /* parent */)
    {
        namespace bf = boost::fusion;
        using PythonHelper::setToDict;

        dict d;
        PythonHelper::apply(bf::begin(src), bf::end(src), [&d, &src](const std::string& name, auto& member)
        {
            if (src.isInitialized.find(name) != src.isInitialized.end())
            {
                setToDict(d, name.c_str(), member);
            }
        });

        return d.inc_ref();
    }
};

template<>
struct type_caster<boost::posix_time::ptime>
{
    static handle cast(const boost::posix_time::ptime& src, return_value_policy /* policy */, handle /* parent */)
    {
        auto d = src.date();
        auto tod = src.time_of_day();
        auto usec = static_cast<int>(tod.total_microseconds() % 1000000);

        PyDateTime_IMPORT;
        return PyDateTime_FromDateAndTime(static_cast<int>(d.year()), 
                                          static_cast<int>(d.month()), 
                                          static_cast<int>(d.day()), 
                                          static_cast<int>(tod.hours()), 
                                          static_cast<int>(tod.minutes()), 
                                          static_cast<int>(tod.seconds()), usec);
    }
};

template<typename T>
struct type_caster<boost::optional<T>>
{
    static handle cast(boost::optional<T> src, return_value_policy policy, handle parent)
    {
        namespace py = pybind11;
        if (src)
        {
            return type_caster<T>::cast(*src, policy, parent);
        }

        // if not set convert to None
        return none().inc_ref();
    }
};

/*!
convert boost::variant to python
from https://blind.guru/boost_python-and-boost_variant.html
*/
template<typename... Ts>
struct type_caster<boost::variant<Ts...>>
{
    struct visitor : boost::static_visitor<handle>
    {
        visitor(return_value_policy policy, handle parent)
            : m_policy(policy), m_parent(parent)
        {}

        template<typename T>
        handle operator()(T const &t) const
        {
            return type_caster<T>::cast(t, m_policy, m_parent);
        }

        return_value_policy m_policy;
        handle              m_parent;
    };

    static handle cast(const boost::variant<Ts...> src, return_value_policy policy, handle parent)
    {
        return boost::apply_visitor(visitor(policy, parent), src);
    }
};

} // namespace detail
} // namespace pybind11
