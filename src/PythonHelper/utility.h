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
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/fusion/sequence/io.hpp>
MSC_RESTORE_WARNINGS
#include <iostream>
#include <vector>
#include <stdexcept>
#include <set>
#include <type_traits>

namespace PythonHelper
{

inline void printDict(const boost::python::dict& dict)
{
	auto start = boost::python::stl_input_iterator<std::string>(dict.keys());
	auto end = boost::python::stl_input_iterator<std::string>();
	std::for_each(start, end, [dict](const std::string& key)
	{
		boost::python::object o(dict[key]);
		std::string object_classname = boost::python::extract<std::string>(o.attr("__class__").attr("__name__"));
		std::cout << key << ": " << object_classname << "\n";
	});
}

inline void printList(const boost::python::list& list)
{
	auto start = boost::python::stl_input_iterator<std::string>(list);
	auto end = boost::python::stl_input_iterator<std::string>();
	std::cout << "[";
	bool first = true;
	std::for_each(start, end, [&first](const std::string& value)
	{
		if (first == false)
		{
			std::cout << ", ";
		}
		first = false;
		std::cout << value;
	});
	std::cout << "]\n";
}

template<typename T>
bool getFromDict(const boost::python::dict& dict, const std::string& key, T& t)
{
	if (dict.has_key(key))
	{
		t = boost::python::extract<T>(dict[key])();
        return true;
	}

    return false;
}

template<typename T>
void setToDict(boost::python::dict& dict, const std::string& key, const T& t)
{
    dict[key] = t;
}

NORETURN(void handlePythonException());

inline void handlePythonException()
{
    using namespace boost::python;

    PyObject *exc, *val, *tb;
    object formatted_list, formatted;
    PyErr_Fetch(&exc, &val, &tb);
    handle<> hexc(exc), hval(allow_null(val)), htb(allow_null(tb));
    object traceback(import("traceback"));

    object format_exception(traceback.attr("format_exception"));
    formatted_list = format_exception(hexc, hval, htb);
    formatted = str("\n").join(formatted_list);

    throw std::runtime_error(extract<std::string>(formatted));
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