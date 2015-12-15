#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "genDefs.h"
#include "typeConverters.h"

BOOST_FUSION_DEFINE_STRUCT(
    (GMusicApi), RadioSeed,
    (std::string, kind)
    (GMusicApi::identifier, albumId)
    (GMusicApi::identifier, artistId)
    (GMusicApi::identifier, genreId)
    (GMusicApi::identifier, trackId)
    (GMusicApi::identifier, trackLockerId)
    )

BOOST_FUSION_DEFINE_STRUCT(
    (GMusicApi), RadioStation,
    (std::string, imageUrl)
    (std::string, kind)
    (std::string, name)
    (bool, deleted)
    (GMusicApi::timestamp, lastModifiedTimestamp)
    (GMusicApi::timestamp, recentTimestamp)
    (GMusicApi::identifier, clientId)
    (GMusicApi::RadioSeed, seed)
    (GMusicApi::identifier, id)
    )

namespace GMusicApi
{

using RadioStationRange = GeneratedRange<RadioStation>;

inline void registerRadioStationConverters()
{
    namespace bp = boost::python;
    // Python to C++ converters
    PyToCppConverter<bp::dict, RadioStation>::registerConverter();
    PyToCppConverter<bp::dict, RadioSeed>::registerConverter();
    PyGeneratorToGeneratedRangeConverter<RadioStation>::registerConverter();
    PySequenceToCppContainerConverter<RadioStationRange>::registerConverter();

    // C++ to Python converters
}

} // namespace GMusicApi