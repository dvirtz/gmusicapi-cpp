#pragma once
#include "gmusicapi/Module.h"
#include "gmusicapi/genDefs.h"

class TestFixture
{
public:
    virtual ~TestFixture() = default;
    static std::string gm_user;
    static std::string gm_pass;
    static std::string gm_refresh;
    static GMusicApi::identifier gm_android_id;
protected:
    GMusicApi::Module m_module;
};