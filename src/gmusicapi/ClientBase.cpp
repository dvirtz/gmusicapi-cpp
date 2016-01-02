#include "ClientBase.h"

namespace GMusicApi
{
ClientBase::ClientBase(const std::string & client_name, 
                       bool debug_logging, 
                       bool validate, 
                       bool verify_ssl)
    : PythonHelper::ClassBase<Module>(client_name, debug_logging, validate, verify_ssl)
{
}

} // namespace GMusicApi
