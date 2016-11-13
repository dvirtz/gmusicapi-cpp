#pragma once

#include "PythonHelper/utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
#include <boost/optional.hpp>
MSC_RESTORE_WARNINGS
#include "gmusicapi/genDefs.h"
#include "PythonHelper/typeConverters.h"

GMUSICAPI_DEFINE_STRUCT(
    (GMusicApi), SharedPlaylistInfo,
    (std::string, author)
    (std::string, description)
    (int, num_tracks)
    (std::string, title)
    )