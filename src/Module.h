#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
MSC_RESTORE_WARNINGS
#include <string>

namespace GMusicApi
{

class Module
{
public:
	static Module& instance();

	template<typename... Args>
	boost::python::object createObject(const std::string& name, Args&&... args) const;

private:
	Module();
	~Module();
	Module(const Module&) = delete;
	Module& operator=(const Module&) = delete;

	void registerTypeConverters();

	boost::python::object m_dict;
};

template<typename ...Args>
boost::python::object Module::createObject(const std::string & name, Args&& ...args) const
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