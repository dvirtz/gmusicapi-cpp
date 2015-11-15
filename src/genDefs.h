#pragma once

/**
general definitions and constants
*/
#include <boost/fusion/include/equal_to.hpp>
#include <boost/fusion/include/not_equal_to.hpp>
#include <boost/fusion/include/io.hpp>

namespace GMusicApi
{

enum
{
	MAX_SONG_RATING = 5 //! maximum rating of a song
};

// used for comparing boost::fusion structs
using boost::fusion::operator==;
using boost::fusion::operator!=;
using boost::fusion::operator<<;

} // namepsace GMusicApi