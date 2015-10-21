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

MSC_DISABLE_WARNINGS
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
MSC_RESTORE_WARNINGS
#include <iostream>

namespace GMusicApi
{

inline void printDict(const boost::python::dict& dict)
{
	auto start = boost::python::stl_input_iterator<std::string>(dict.keys());
	auto end = boost::python::stl_input_iterator<std::string>();
	std::for_each(start, end, [dict](std::string key)
	{
		boost::python::object o(dict[key]);
		std::string object_classname = boost::python::extract<std::string>(o.attr("__class__").attr("__name__"));
		std::cout << key << ": " << object_classname << "\n";
	});
}

template<typename T>
void getFromDict(const boost::python::dict& dict, const std::string& key, T& t)
{
	if (dict.has_key(key))
	{
		t = boost::python::extract<T>(dict[key]);
	}
}
} // namespace GMusicApi
