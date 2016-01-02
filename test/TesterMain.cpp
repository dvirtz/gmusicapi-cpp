#define CATCH_CONFIG_RUNNER 
#include "catch.hpp"
#include "initialize.h"

int main(int argc, char* argv[])
{
//    PythonHelper::initialize();
    return Catch::Session().run(argc, argv);
}