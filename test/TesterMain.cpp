#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "Module.h"
#include "Musicmanager.h"

int main(int argc, char* argv[])
{
    Catch::Session session;

    auto returnCode = session.applyCommandLine(argc, argv, Catch::Session::OnUnusedOptions::Ignore);
    if (returnCode != 0) // Indicates a command line error
        return returnCode;

    auto unusedOptions = session.unusedTokens();
    for (auto& option : session.unusedTokens())
    {
        if (option.data == "wait")
        {
            std::string s;
            std::cout << "Press enter to start testing.\n";
            std::cin >> s;
        }
        else if (option.data == "oauth")
        {
            namespace gm = GMusicApi;
            gm::Module m;
            gm::Musicmanager mm(m);
            mm.perform_oauth();
        }
    }
    
    return session.run();
}
