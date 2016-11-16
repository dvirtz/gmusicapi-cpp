#include "gmusicapi/Module.h"
#include "gmusicapiPath.h"

namespace GMusicApi
{

Module::Module()
    : PythonHelper::ModuleBase("gmusicapi", gmusicapi_path)
{
}

} // namespace GMusicApi
