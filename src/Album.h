#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "genDefs.h"
#include "typeConverters.h"

BOOST_FUSION_DEFINE_STRUCT(
    (GMusicApi), Album,
    (std::string, albumArtRef)
    (std::string, albumArtist)
    (GMusicApi::identifier, albumId)
    (std::string, artist)
    (GMusicApi::identifiers, artistId)
    (std::string, kind)
    (std::string, name)
    (int, year)
    );

namespace GMusicApi
{

inline void registerAlbumConverters()
{
    namespace bp = boost::python;
    // Python to C++ converters
    PyToCppConverter<bp::dict, Album>::registerConverter();
    PySequenceToCppContainerConverter<std::vector<Album>>::registerConverter();

    // C++ to Python converters
}

} // namespace GMusicApi