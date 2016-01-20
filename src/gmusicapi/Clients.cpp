#include "Clients.h"
#include "gmusicapiPath.h"

namespace GMusicApi
{

Clients::Clients()
    : PythonHelper::ModuleBase<Clients>("gmusicapi.clients", gmusicapi_path)
{}

} // namespace GMusicApi