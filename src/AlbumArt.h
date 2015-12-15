#pragma once
#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/fusion/include/define_struct.hpp>
MSC_RESTORE_WARNINGS
#include "genDefs.h"
#include "typeConverters.h"
#include <string>

BOOST_FUSION_DEFINE_STRUCT(
    (GMusicApi), AlbumArt,
    (std::string, url)
    )

namespace GMusicApi
{

inline void registerAlbumArtConverters()
{
    namespace bp = boost::python;
    // Python to C++ converters
	PyToCppConverter<bp::dict, AlbumArt>::registerConverter();
    PySequenceToCppContainerConverter<std::vector<AlbumArt>>::registerConverter();

    // C++ to Python converters
	bp::to_python_converter<std::vector<AlbumArt>, CppContainerToPyListConverter<std::vector<AlbumArt>>>();
    bp::to_python_converter<AlbumArt, StructToPyDictConverter<AlbumArt>>();
}

} // namespace GMusicApi