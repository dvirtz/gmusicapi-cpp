#pragma once
#include <mutex>

namespace PythonHelper
{

class Initializer
{
public:
    Initializer();
    
private:

    static void registerTypeConverters();

    static std::once_flag m_onceFlag;
};

} // namespace PythonHelper
