#pragma once
#include "gmusicapi/Module.h"

class TestFixture
{
public:
    virtual ~TestFixture() = default;
protected:
    GMusicApi::Module m_module;
};