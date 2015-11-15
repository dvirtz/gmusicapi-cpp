#pragma once
#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/adapted/struct/define_struct.hpp>
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "genDefs.h"
#include <string>

BOOST_FUSION_DEFINE_STRUCT(
    (GMusicApi), AlbumArt,
    (std::string, url)
    )