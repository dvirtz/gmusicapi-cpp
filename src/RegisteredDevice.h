#pragma once
#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/python.hpp>
MSC_RESTORE_WARNINGS

namespace GMusicApi
{

struct RegisteredDevice
{
	RegisteredDevice(const boost::python::dict& dict)
	{
		getFromDict(dict, "kind", m_kind);
		getFromDict(dict, "id", m_id);
		getFromDict(dict, "friendlyName", m_friendlyName);
		getFromDict(dict, "type", m_type);
		getFromDict(dict, "lastAccessedTimeMs", m_lastAccessedTimeMs);
		getFromDict(dict, "smartPhone", m_isSmartphone);
	}

	std::string m_kind;
	std::string m_id;
	std::string m_friendlyName;
	std::string m_type;
	std::string	m_lastAccessedTimeMs;
	bool		m_isSmartphone = false;
};

} // namespace GMusicApi