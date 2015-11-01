#pragma once
#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/python.hpp>
MSC_RESTORE_WARNINGS
#include "DictionaryValue.h"

namespace GMusicApi
{

struct RegisteredDevice
{
	RegisteredDevice(const boost::python::dict& dict)
		: m_dict(dict),
		m_kind(dict, "kind"),
		m_id(dict, "id"),
		m_friendlyName(dict, "friendlyName"),
		m_type(dict, "type"),
		m_lastAccessedTimeMs(dict, "lastAccessedTimeMs"),
		m_isSmartphone(dict, "smartPhone")
	{		
	}

	boost::python::dict				m_dict;
	DictionaryValue<std::string>	m_kind;
	DictionaryValue<std::string>	m_id;
	DictionaryValue<std::string>	m_friendlyName;
	DictionaryValue<std::string>	m_type;
	DictionaryValue<std::string>	m_lastAccessedTimeMs;
	DictionaryValue<bool>			m_isSmartphone;
};

} // namespace GMusicApi