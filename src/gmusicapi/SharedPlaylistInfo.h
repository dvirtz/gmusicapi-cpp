#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
#include <boost/optional.hpp>
MSC_RESTORE_WARNINGS
#include "genDefs.h"
#include "typeConverters.h"

GMUSICAPI_DEFINE_STRUCT(
    (GMusicApi), SharedPlaylistInfo,
    (std::string, author)
    (std::string, description)
    (int, num_tracks)
    (std::string, title)
    )

namespace GMusicApi
{

inline void registerSharedPlaylistInfoConverters()
{
    namespace bp = boost::python;
    namespace ph = PythonHelper;

    // Python to C++ converters
    ph::PyToCppConverter<bp::dict, SharedPlaylistInfo>::registerConverter();
}

} // namespace GMusicApi