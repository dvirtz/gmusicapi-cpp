#pragma once
#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/python.hpp>
MSC_RESTORE_WARNINGS
#include "DictionaryValue.h"

namespace GMusicApi
{

struct AlbumArt
{
	AlbumArt(const boost::python::dict& dict)
		: m_dict(dict),
		m_url(dict, "url")
	{
	}

	boost::python::dict				m_dict;
	DictionaryValue<std::string>	m_url;
};

} // namespace GMusicApi