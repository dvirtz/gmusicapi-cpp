#include "PythonHelper/Initializer.h"
#include "PythonHelper/utility.h"
MSC_DISABLE_WARNINGS
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
MSC_RESTORE_WARNINGS
#include "PythonHelper/typeConverters.h"

namespace PythonHelper
{

std::once_flag Initializer::m_onceFlag;

Initializer::Initializer()
{
    // execute this code only once
    std::call_once(m_onceFlag, []()
    {
        Py_Initialize();

        Initializer::registerTypeConverters();
    });
}

void Initializer::registerTypeConverters()
{
    namespace bp = boost::python;

    // Python to C++ converters
    PySequenceToCppContainerConverter<std::vector<std::string>>::registerConverter();
    PyToCppConverter<bp::str, std::string>::registerConverter();

    // C++ to Python converters
    bp::to_python_converter<nullptr_t, NullptrToNoneConverter>();
    bp::to_python_converter<std::vector<std::string>, CppContainerToPyListConverter<std::vector<std::string>>>();
    bp::to_python_converter<boost::posix_time::ptime, BoostPTimeToPyDateTimeConverter>();
    bp::to_python_converter<boost::optional<std::string>, BoostOptionalToPyConverter<std::string>>();
    bp::to_python_converter<boost::optional<bool>, BoostOptionalToPyConverter<bool>>();
    bp::to_python_converter<boost::optional<boost::posix_time::ptime>, BoostOptionalToPyConverter<boost::posix_time::ptime>>();
    bp::to_python_converter<boost::variant<int, std::string>, BoostVariantToPyConverter<int, std::string>>();
}

} // namespace PythonHelper
