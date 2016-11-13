#include "PythonHelper/ModuleBase.h"
#include "PythonHelper/Initializer.h"
#include "PythonHelper/typeConverters.h"

namespace PythonHelper
{

ModuleBase::ModuleBase(const char* name, const boost::optional<std::string>& path)
{
    namespace py = pybind11;

    static Initializer initializer;

    try
    {
        if (path)
        {
            auto sys = py::module::import("sys");
            py::list pythonPath(sys.attr("path"));

            // add module to path
            pythonPath.append(py::str(*path));
        }

        // import gmusicapi module
        auto module = py::module::import(name);
        m_dict = module.attr("__dict__");
    }
    catch (const py::error_already_set&)
    {
        handlePythonException();
    }
}

ModuleBase::~ModuleBase() = default;

} // namespace PythonHelper