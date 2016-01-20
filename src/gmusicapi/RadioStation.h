#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "genDefs.h"
#include "typeConverters.h"

GMUSICAPI_DEFINE_STRUCT(
    (GMusicApi), RadioSeed,
    (std::string, kind)
    (GMusicApi::identifier, albumId)
    (GMusicApi::identifier, artistId)
    (GMusicApi::identifier, genreId)
    (GMusicApi::identifier, trackId)
    (GMusicApi::identifier, trackLockerId)
    )

GMUSICAPI_DEFINE_STRUCT(
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
    namespace ph = PythonHelper;

    // Python to C++ converters
    ph::PyToCppConverter<bp::dict, RadioStation>::registerConverter();
    ph::PyToCppConverter<bp::dict, RadioSeed>::registerConverter();
    ph::PySequenceToCppContainerConverter<RadioStationRange>::registerConverter();

    PyGeneratorToGeneratedRangeConverter<RadioStation>::registerConverter();

    // C++ to Python converters
}

} // namespace GMusicApi