#include "Catch.hpp"
#include "Webclient.h"

using namespace GMusicApi;

TEST_CASE("Webclient constructed", "[Webclient]")
{
	REQUIRE_NOTHROW(Webclient w);
}