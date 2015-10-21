#pragma once
#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/python.hpp>
MSC_RESTORE_WARNINGS

namespace GMusicApi
{

struct AlbumArt
{
	AlbumArt(const boost::python::dict& dict)
	{
		getFromDict(dict, "url", m_url);
	}

	std::string m_url;
};

} // namespace GMusicApi