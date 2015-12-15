#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "genDefs.h"

BOOST_FUSION_DEFINE_STRUCT(
    (GMusicApi), Artist,
    (std::vector<GMusicApi::Album>, albums)
    (std::string, artistArtRef)
    (GMusicApi::identifier, artistId)
    (std::string, kind)
    (std::string, name)
    (std::vector<GMusicApi::Artist>, related_artists)
    (std::vector<GMusicApi::Song>, topTracks)
    (int, total_albums)
    )

namespace GMusicApi
{

inline void registerArtistConverters()
{
    namespace bp = boost::python;
    // Python to C++ converters
    PyToCppConverter<bp::dict, Artist>::registerConverter();
    PySequenceToCppContainerConverter<std::vector<Artist>>::registerConverter();

    // C++ to Python converters
}

} // namespace GMusicApi
