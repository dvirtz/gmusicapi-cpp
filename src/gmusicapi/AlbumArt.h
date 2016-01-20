#pragma once
#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "genDefs.h"
#include "typeConverters.h"
#include <string>

GMUSICAPI_DEFINE_STRUCT(
    (GMusicApi), AlbumArt,
    (std::string, url)
    )

namespace GMusicApi
{

inline void registerAlbumArtConverters()
{
    namespace bp = boost::python;
    namespace ph = PythonHelper;

    // Python to C++ converters
    ph::PyToCppConverter<bp::dict, AlbumArt>::registerConverter();
    ph::PySequenceToCppContainerConverter<std::vector<AlbumArt>>::registerConverter();

    // C++ to Python converters
	bp::to_python_converter<std::vector<AlbumArt>, ph::CppContainerToPyListConverter<std::vector<AlbumArt>>>();
    bp::to_python_converter<AlbumArt, ph::StructToPyDictConverter<AlbumArt>>();
}

} // namespace GMusicApi