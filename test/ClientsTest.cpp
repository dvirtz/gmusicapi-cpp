#include "catch.hpp"
#include "TestFixture.h"
#include "gmusicapi/Clients.h"

using namespace GMusicApi;

class ClientsTestFixture
{
protected:
    Clients m_module;
};

TEST_CASE_METHOD(ClientsTestFixture, "OAUTH_FILEPATH is not empty", "[Musicmanager]")
{
    REQUIRE_FALSE(m_module.OAUTH_FILEPATH().empty());
}