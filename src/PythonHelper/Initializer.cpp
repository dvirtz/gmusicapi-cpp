#include "PythonHelper/Initializer.h"
#include <pybind11/pybind11.h>

namespace PythonHelper
{

std::once_flag Initializer::m_onceFlag;

Initializer::Initializer()
{
    // execute this code only once
    std::call_once(m_onceFlag, []()
    {
        Py_Initialize();
    });
}

} // namespace PythonHelper
