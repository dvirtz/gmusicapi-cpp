#pragma once

#include "ModuleBase.h"

namespace GMusicApi
{

class Module : public PythonHelper::ModuleBase<Module>
{
protected:
    Module();

    void registerTypeConverters();

    friend class PythonHelper::ModuleBase<Module>;
};

} // namespace GMusicApi