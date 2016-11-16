#pragma once

#ifdef _MSC_VER
#define MSC_DISABLE_WARNINGS \
__pragma(warning(push, 0))
#define MSC_RESTORE_WARNINGS \
__pragma(warning(pop))
#else
#define MSC_DISABLE_WARNINGS
#define MSC_RESTORE_WARNINGS
#endif // MSC_VER

#ifdef _MSC_VER
#define NORETURN(...) __declspec(noreturn) __VA_ARGS__
#else
#define NORETURN(...) __VA_ARGS__ __attribute__ ((noreturn))
#endif

MSC_DISABLE_WARNINGS
#include <pybind11/pybind11.h>
#include <boost/fusion/sequence/io.hpp>
MSC_RESTORE_WARNINGS
#include <iostream>
#include <vector>
#include <stdexcept>
#include <set>
#include <type_traits>
#include <iterator>

namespace PythonHelper
{

inline void printDict(const pybind11::dict& dict)
{
    pybind11::print(dict);
}

inline void printList(const pybind11::list& list)
{
    pybind11::print(list);
}

template<typename T>
bool getFromDict(const pybind11::dict& dict, const char* key, T& t)
{
	if (dict.contains(key))
	{
        t = dict[key].cast<T>();
        return true;
	}

    return false;
}

template<typename T>
void setToDict(pybind11::dict& dict, const char* key, const T& t)
{
    dict[key] = pybind11::cast(t);
}

template<typename T>
struct is_sequence : public std::false_type {};

template<typename T>
struct is_sequence<std::vector<T>> : public std::true_type {};

template<typename T>
struct is_sequence<std::set<T>> : public std::true_type {};

/*!
print sequences
*/
template <typename Sequence>
inline typename
boost::enable_if<
    is_sequence<Sequence>
    , std::ostream&
>::type operator<<(std::ostream& out, const Sequence& s)
{
    out << '[';
    std::copy(s.begin(), s.end(), std::ostream_iterator<typename Sequence::value_type>(out, ", "));
    out << "\b\b]"; // erase last comma
    return out;
}

/*!
convert sequence to vector
*/
template<typename Sequence>
auto toVector(const Sequence& range)
{
    return std::vector<typename Sequence::value_type>(range.begin(), range.end());
}

} // namespace PythonHelper

namespace std
{
using PythonHelper::operator<<;
}