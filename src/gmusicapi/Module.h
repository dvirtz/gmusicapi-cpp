#pragma once

#include "ModuleBase.h"

namespace GMusicApi
{

class Module : public PythonHelper::ModuleBase<Module>
{
public:
    Module();

private:
	void registerTypeConverters();
};

} // namespace GMusicApi