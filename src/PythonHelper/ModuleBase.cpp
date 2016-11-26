#include "PythonHelper/ModuleBase.h"
#include "PythonHelper/Initializer.h"
#include "PythonHelper/typeConverters.h"
#include <pybind11/eval.h>

namespace PythonHelper
{

ModuleBase::ModuleBase(const char* name, const boost::optional<std::string>& path)
{
    namespace py = pybind11;

    static Initializer initializer;

    if (path)
    {
        auto sys = py::module::import("sys");
        py::list pythonPath(sys.attr("path"));

        // add module to path
        pythonPath.append(py::str(*path));
    }

    // import requested module
    auto module = py::module::import(name);
    m_dict = py::dict(module.attr("__dict__"));
}

ModuleBase::~ModuleBase() = default;

} // namespace PythonHelper