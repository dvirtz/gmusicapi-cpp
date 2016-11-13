#pragma once
#include "PythonHelper/utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "gmusicapi/genDefs.h"
#include "PythonHelper/typeConverters.h"
#include <string>

GMUSICAPI_DEFINE_STRUCT(
    (GMusicApi), AlbumArt,
    (std::string, url)
    )