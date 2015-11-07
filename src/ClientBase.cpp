#include "ClientBase.h"

namespace GMusicApi
{
ClientBase::ClientBase(const std::string & client_name, bool debug_logging, bool validate, bool verify_ssl)
    : m_object(instance().createObject(client_name, debug_logging, validate, verify_ssl))
{
}

bool ClientBase::validate() const
{
    return getMember<bool>("validate");
}

void ClientBase::set_validate(bool val)
{
    setMember("validate", val);
}

GMusicApi& ClientBase::instance() const
{
	return GMusicApi::instance();
}

} // namespace GMusicApi
