#pragma once
#include "ModuleBase.h"
#include "ClassMember.h"

namespace GMusicApi
{

class Clients : public PythonHelper::ModuleBase<Clients>
{
public:
    CONST_CLASS_MEMBER(std::string, OAUTH_FILEPATH, Clients);

protected:
    Clients();

    friend class PythonHelper::ModuleBase<Clients>;
};

} // namespace GMusicApi