#include "GMusicApi.h"
#include "typeConverters.h"
#include "RegisteredDevice.h"
#include "gmusicapiPath.h"
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
	// must be called before dilling with python
	Py_Initialize();

	try
	{
		// add gmusicapi module to path
		auto sys = import("sys");
		list path = extract<list>(sys.attr("path"));
		path.append(gmusicapi_path);

		// import gmusicapi module
		auto module = import("gmusicapi");
		m_dict = module.attr("__dict__");
	}
	catch (const error_already_set&)
	{
		handlePythonException();
	}

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

void GMusicApi::handlePythonException() const
{
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

} // namespace GMusicApi