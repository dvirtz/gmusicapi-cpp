#include "Musicmanager.h"

namespace GMusicApi
{

Musicmanager::Musicmanager(bool debug_logging, 
                           bool validate, 
                           bool verify_ssl)
    : ClientBase("Musicmanager", debug_logging, validate, verify_ssl)
{}

void Musicmanager::perform_oauth(const boost::optional<std::string>& storage_filepath, 
                                 bool open_browser)
{
    callStaticMethod<void>("Musicmanager", "perform_oauth", storage_filepath, open_browser);
}

} // namespace GMusicApi
