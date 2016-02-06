#pragma once
#include "ModuleBase.h"

namespace GMusicApi
{

class Clients : public PythonHelper::ModuleBase
{
public:
    Clients();

    std::string OAUTH_FILEPATH();
};

} // namespace GMusicApi