#include "GMusicApi.h"
#include "typeConverters.h"
#include "RegisteredDevice.h"
#include <iostream>

namespace GMusicApi
{

using namespace boost::python;

GMusicApi& GMusicApi::instance()
{
	static GMusicApi instance;
	return instance;
}

GMusicApi::GMusicApi()
{
	Py_Initialize();
	auto module = import("gmusicapi");
	m_dict = module.attr("__dict__");

	registerTypeConverters();
}


GMusicApi::~GMusicApi()
{
}

void GMusicApi::registerTypeConverters()
{
	pySequenceToCppContainerConverter<SongRange>::registerConverter();
	pyGeneratorToSongRangeConverter::registerConverter();
	pyToCppConverter<Song, boost::python::dict>::registerConverter();
	pySequenceToCppContainerConverter<std::vector<std::string>>::registerConverter();
	pyToCppConverter<std::string, boost::python::str>::registerConverter();
	pyToCppConverter<AlbumArt, boost::python::dict>::registerConverter();
	pySequenceToCppContainerConverter<std::vector<AlbumArt>>::registerConverter();
	boost::python::to_python_converter<nullptr_t, nullptrToNoneConverter>();
	pySequenceToCppContainerConverter<std::vector<RegisteredDevice>>::registerConverter();
	pyToCppConverter<RegisteredDevice, boost::python::dict>::registerConverter();
}

} // namespace GMusicApi