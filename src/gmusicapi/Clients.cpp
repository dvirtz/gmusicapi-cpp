#include "gmusicapi/Clients.h"

namespace GMusicApi
{

Clients::Clients()
    : PythonHelper::ModuleBase("gmusicapi.clients")
{}

std::string Clients::OAUTH_FILEPATH()
{
    return getMember<std::string>("OAUTH_FILEPATH");
}

} // namespace GMusicApi