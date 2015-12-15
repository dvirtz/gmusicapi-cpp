#pragma once
#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "genDefs.h"
#include "typeConverters.h"
#include <string>

BOOST_FUSION_DEFINE_STRUCT(
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
    // Python to C++ converters
	PySequenceToCppContainerConverter<std::vector<RegisteredDevice>>::registerConverter();
    PyToCppConverter<bp::dict, RegisteredDevice>::registerConverter();

    // C++ to Python converters
	bp::to_python_converter<RegisteredDevice, StructToPyDictConverter<RegisteredDevice>>();
}

} // namespace GMusicApi