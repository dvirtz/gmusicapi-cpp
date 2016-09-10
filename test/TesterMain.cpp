#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "gmusicapi/Module.h"
#include "gmusicapi/Musicmanager.h"
#include "TestFixture.h"
#include <boost/program_options.hpp>
#include <algorithm>

int main(int argc, char* argv[])
{
    namespace po = boost::program_options;

    bool wait;
    bool oauth;

    po::options_description desc("Other options");
    desc.add_options()
        ("help,h", "produce help message")
        ("username", po::value(&TestFixture::gm_user)->required(), "Google Music username")
        ("password", po::value(&TestFixture::gm_pass)->required(), "Google Music password")
        ("refresh_token", po::value(&TestFixture::gm_refresh), "Google Music refresh token")
        ("android_id", po::value(&TestFixture::gm_user), "Google Music Android ID")
        ("wait", po::value(&wait)->default_value(false), "Wait for keystroke before starting")
        ("oauth", po::value(&oauth)->default_value(false), "Perform OAuth")
        ;

    po::variables_map vm;
    auto parsedOptions = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
    po::store(parsedOptions, vm);

    Catch::Session session;

    auto usage = [&]()
    {
        session.showHelp(argv[0]);

        Catch::cout() << desc << '\n';
    };

    try
    {
        po::notify(vm);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << '\n';
        usage();
        return 1;
    }

    if (vm.count("help"))
    {
        usage();
        return 0;
    }

    if (wait)
    {
        std::string s;
        std::cout << "Press enter to start testing.\n";
        std::cin >> s;
    }

    if (oauth)
    {
        namespace gm = GMusicApi;
        gm::Module m;
        gm::Musicmanager mm(m);
        mm.perform_oauth();
    }

    auto unusedOptions = collect_unrecognized(parsedOptions.options, po::include_positional);

    // convert to const char*
    std::vector<const char*> unusedOptionsToCatch = { argv[0] };
    std::transform(unusedOptions.begin(), unusedOptions.end(), std::back_inserter(unusedOptionsToCatch),
                   [](const std::string& str)
    {
        return str.c_str();
    });
    return session.run(unusedOptionsToCatch.size(), unusedOptionsToCatch.data());
}
