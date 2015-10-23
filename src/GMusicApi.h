#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
MSC_RESTORE_WARNINGS
#include <string>
#include <sstream>
#include <map>
#include <exception>

namespace GMusicApi
{

class GMusicApi
{
public:
	static GMusicApi& instance();

	template<typename... Args>
	boost::python::object createObject(const std::string& name, Args&&... args) const;

	template<typename R, typename... Args>
	R callMethod(const boost::python::object& object, const std::string& methodName, Args&&... args) const;

private:
	GMusicApi();
	~GMusicApi();
	GMusicApi(const GMusicApi&) = delete;
	GMusicApi& operator=(const GMusicApi&) = delete;

	void registerTypeConverters();

	void handlePythonException() const;

	boost::python::object m_dict;
};

template<typename ...Args>
boost::python::object GMusicApi::createObject(const std::string & name, Args&& ...args) const
{
	namespace bp = boost::python;
	try
	{
		bp::object type = m_dict[name];
		return bp::call<boost::python::object>(type.ptr(), std::forward<Args>(args)...);
	}
	catch (const bp::error_already_set&)
	{
		handlePythonException();
		return bp::object();
	}
}

template<typename R, typename ...Args>
R GMusicApi::callMethod(const boost::python::object& object, const std::string & methodName, Args && ...args) const
{
	namespace bp = boost::python;
	try
	{
		return boost::python::call_method<R>(object.ptr(), methodName.c_str(), std::forward<Args>(args)...);
	}
	catch (const bp::error_already_set&)
	{
		handlePythonException();
		return R();
	}
}

} // namespace GMusicApi