#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "gmusicapi/Module.h"
#include "gmusicapi/Musicmanager.h"
#include "TestFixture.h"
#include <boost/program_options.hpp>
#include <algorithm>

std::string getEnv(const char* envVar)
{
#ifdef _MSC_VER
    size_t requiredSize;
    getenv_s(&requiredSize, nullptr, 0, envVar);
    if (requiredSize == 0)
        return std::string();

    // std::string doesn't need terminating null
    std::string res(requiredSize - 1, 0);
    getenv_s(&requiredSize, &res[0], requiredSize, envVar);
    return res;
#else
    auto res = getenv(envVar);
    return res ? res : std::string();
#endif
}

int main(int argc, char* argv[])
{
    namespace po = boost::program_options;

    struct gm_auth_token
    {
        const char*     m_name;
        const char*     m_description;
        const char*     m_environment;
        std::string*    m_pVariable;
        bool            m_isRequired;
    };

    static const std::vector<gm_auth_token> gm_auto_tokens =
    {
        {"username", "Google Music username", "GM_U", &TestFixture::gm_user, true},
        {"password", "Google Music password", "GM_P", &TestFixture::gm_pass, true},
        {"refresh_token", "Google Music refresh token", "GM_R", &TestFixture::gm_refresh, false},
        {"android_id", "Google Music Android ID", "GM_AA_ID", &TestFixture::gm_android_id, false}
    };

    po::options_description desc("Other options");
    desc.add_options()
        ("help,h", "produce help message")
        ("wait", "Wait for keystroke before starting")
        ("oauth", "Perform OAuth")
        ;
    for (auto& auth_token : gm_auto_tokens)
    {
        desc.add_options()
            (auth_token.m_name, po::value(auth_token.m_pVariable), auth_token.m_description);
    }

    po::variables_map vm;

    Catch::Session session;

    auto usage = [&]()
    {
        session.showHelp(argv[0]);

        Catch::cout() << desc << '\n';
    };

    auto parsedOptions = [&]()
    {
        try
        {
            auto parsedOptions = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
            po::store(parsedOptions, vm);
            po::notify(vm);
            return parsedOptions;
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << '\n';
            std::cerr << "run " << argv[0] << "-h" << " for more info.\n";
            exit(1);
        }
    }();

    if (vm.count("help"))
    {
        usage();
        return 0;
    }

    if (vm.count("oauth"))
    {
        namespace gm = GMusicApi;
        gm::Module m;
        gm::Musicmanager mm(m);
        mm.perform_oauth(gm::Clients().OAUTH_FILEPATH(), true);
        return 0;
    }

    for (auto& auth_token : gm_auto_tokens)
    {
        if (auth_token.m_pVariable->empty())
        {
            auth_token.m_pVariable->assign(getEnv(auth_token.m_environment));

            if (auth_token.m_isRequired && auth_token.m_pVariable->empty())
            {
                std::cerr << "Auth token " << auth_token.m_name << " should be given either on the command line, "
                    << " or set on the environment variable " << auth_token.m_environment << ".\n";
                std::cerr << "run " << argv[0] << "-h" << " for more info.\n";
                return 1;
            }
        }
    }

    if (vm.count("wait"))
    {
        std::cout << "Press enter to start testing.\n";
        std::cin.get();
    }

    auto unusedOptions = po::collect_unrecognized(parsedOptions.options, po::include_positional);

    // convert to const char*
    std::vector<const char*> unusedOptionsToCatch = { argv[0] };
    std::transform(unusedOptions.begin(), unusedOptions.end(), std::back_inserter(unusedOptionsToCatch),
                   [](const std::string& str)
    {
        return str.c_str();
    });
    return session.run(static_cast<int>(unusedOptionsToCatch.size()), unusedOptionsToCatch.data());
}
