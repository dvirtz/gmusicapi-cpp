#pragma once

#include "PythonHelper/utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "gmusicapi/genDefs.h"
#include "PythonHelper/typeConverters.h"
#include "gmusicapi/AlbumArt.h"
#include "gmusicapi/GeneratedRange.h"

GMUSICAPI_DEFINE_STRUCT(
    (GMusicApi), Genre,
    (std::string, name)
    (GMusicApi::identifier, id)
    (std::string, kind)
    (GMusicApi::identifiers, children)
    (std::vector<GMusicApi::AlbumArt>, images)
    )

namespace GMusicApi
{
using GenreRange = GeneratedRange<Genre>;

inline void registerGenreConverters()
{
    namespace bp = boost::python;
    namespace ph = PythonHelper;

    // Python to C++ converters
    ph::PyToCppConverter<bp::dict, Genre>::registerConverter();
    ph::PySequenceToCppContainerConverter<GenreRange>::registerConverter();

    PyGeneratorToGeneratedRangeConverter<Genre>::registerConverter();

    // C++ to Python converters
}

} // namespace GMusicApi#pragma once
