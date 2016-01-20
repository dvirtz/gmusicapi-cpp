#pragma once

/**
general definitions and constants
*/
#include <boost/fusion/include/equal_to.hpp>
#include <boost/fusion/include/not_equal_to.hpp>
#include <boost/fusion/include/io.hpp>
#include <string>
#include <vector>
#include <map>

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

using identifier        = std::string;
using identifiers       = std::vector<identifier>;
using timestamp         = std::string;
using UploadResult      = std::map<std::string, std::string>;
using UploadResults     = std::tuple<UploadResult, UploadResult, UploadResult>;
using DownloadResult    = std::pair<std::string, std::string>;

#define GMUSICAPI_DEFINE_STRUCT(NAMESPACE_SEQ, NAME, ATTRIBUTES)    \
    BOOST_FUSION_DEFINE_STRUCT(NAMESPACE_SEQ, NAME, ATTRIBUTES(std::set<std::string>, isInitialized))

} // namepsace GMusicApi