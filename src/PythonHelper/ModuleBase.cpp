#include "PythonHelper/ModuleBase.h"

namespace PythonHelper
{

ModuleBase::ModuleBase(const std::string & name, const boost::optional<std::string>& path)
{
    namespace bp = boost::python;

    static Initializer initializer;

    try
    {
        if (path)
        {
            // add module to path
            auto sys = bp::import("sys");
            bp::list pythonPath = bp::extract<bp::list>(sys.attr("path"));
            pythonPath.append(*path);
        }

        // import gmusicapi module
        auto module = bp::import(name.c_str());
        m_dict = module.attr("__dict__");
    }
    catch (const bp::error_already_set&)
    {
        handlePythonException();
    }
}

ModuleBase::~ModuleBase() = default;

} // namespace PythonHelper