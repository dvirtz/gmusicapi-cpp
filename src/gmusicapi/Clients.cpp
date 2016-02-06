#include "Clients.h"
#include "gmusicapiPath.h"

namespace GMusicApi
{

Clients::Clients()
    : PythonHelper::ModuleBase("gmusicapi.clients", gmusicapi_path)
{}

std::string Clients::OAUTH_FILEPATH()
{
    return getMember<std::string>("OAUTH_FILEPATH");
}

} // namespace GMusicApi