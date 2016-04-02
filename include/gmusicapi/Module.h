#pragma once

#include "PythonHelper/ModuleBase.h"
#include <mutex>

namespace GMusicApi
{

class Module : public PythonHelper::ModuleBase
{
public:
    Module();

protected:
    static void registerTypeConverters();

    static std::once_flag m_onceFlag;
};

} // namespace GMusicApi