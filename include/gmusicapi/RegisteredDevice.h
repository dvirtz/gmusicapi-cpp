#pragma once
#include "PythonHelper/utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "gmusicapi/genDefs.h"
#include "PythonHelper/typeConverters.h"
#include <string>

GMUSICAPI_DEFINE_STRUCT(
    (GMusicApi), RegisteredDevice,
    (std::string, kind)
    (GMusicApi::identifier, id)
    (std::string, friendlyName)
    (std::string, type)
    (GMusicApi::timestamp, lastAccessedTimeMs)
    (bool, isSmartphone)
    )

namespace GMusicApi
{

inline void registerRegisteredDeviceConverters()
{
    namespace bp = boost::python;
    namespace ph = PythonHelper;

    // Python to C++ converters
	ph::PySequenceToCppContainerConverter<std::vector<RegisteredDevice>>::registerConverter();
    ph::PyToCppConverter<bp::dict, RegisteredDevice>::registerConverter();

    // C++ to Python converters
	bp::to_python_converter<RegisteredDevice, ph::StructToPyDictConverter<RegisteredDevice>>();
}

} // namespace GMusicApi