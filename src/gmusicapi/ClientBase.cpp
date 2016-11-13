#include "gmusicapi/ClientBase.h"
#include "gmusicapi/Module.h"

namespace GMusicApi
{
ClientBase::ClientBase(Module& module,
                       const char* client_name,
                       bool debug_logging, 
                       bool validate, 
                       bool verify_ssl)
    : PythonHelper::ClassBase(module, client_name, debug_logging, validate, verify_ssl)
{
}

bool ClientBase::validate()
{
    return getMember<bool>("validate");
}

void ClientBase::setValidate(bool val)
{
    setMember("validate", val);
}

} // namespace GMusicApi
