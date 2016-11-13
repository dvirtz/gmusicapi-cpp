#include "gmusicapi/Song.h"
#include <pybind11/pybind11.h>

static_assert(std::is_const<typename boost::iterators::iterator_reference<GMusicApi::TypedIterator<GMusicApi::Song>>::type>::value, "const");

void foo()
{
    pybind11::object().cast<GMusicApi::SongRange>();
}