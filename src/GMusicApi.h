#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
MSC_RESTORE_WARNINGS
#include <string>

namespace GMusicApi
{

class GMusicApi
{
public:
	static GMusicApi& instance();

	template<typename... Args>
	boost::python::object createObject(const std::string& name, Args&&... args) const;

private:
	GMusicApi();
	~GMusicApi();
	GMusicApi(const GMusicApi&) = delete;
	GMusicApi& operator=(const GMusicApi&) = delete;

	void registerTypeConverters();

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
	}
}

} // namespace GMusicApi