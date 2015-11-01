#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/python.hpp>
MSC_RESTORE_WARNINGS
#include "AlbumArt.h"
#include "GeneratedRange.h"
#include "DictionaryValue.h"
#include <string>
#include <vector>

namespace GMusicApi
{

struct Song
{
	Song(const boost::python::dict& dict)
		: m_dict(dict),
		m_comment(dict, "comment"),
		m_rating(dict, "rating"),
		m_albumArtRef(dict, "albumArtRef"),
		m_artistId(dict, "artistId"),
		m_composer(dict, "composer"),
		m_year(dict, "year"),
		m_creationTimestamp(dict, "creationTimestamp"),
		m_id(dict, "id"),
		m_album(dict, "album"),
		m_totalDiscCount(dict, "totalDiscCount"),
		m_title(dict, "title"),
		m_recentTimestamp(dict, "recentTimestamp"),
		m_albumArtist(dict, "albumArtist"),
		m_trackNumber(dict, "trackNumber"),
		m_discNumber(dict, "discNumber"),
		m_deleted(dict, "deleted"),
		m_nid(dict, "nid"),
		m_totalTrackCount(dict, "totalTrackCount"),
		m_estimatedSize(dict, "estimatedSize"),
		m_albumId(dict, "albumId"),
		m_beatsPerMinute(dict, "beatsPerMinute"),
		m_genre(dict, "genre"),
		m_playCount(dict, "playCount"),
		m_artistArtRef(dict, "artistArtRef"),
		m_kind(dict, "kind"),
		m_artist(dict, "artist"),
		m_lastModifiedTimestamp(dict, "lastModifiedTimestamp"),
		m_clientId(dict, "clientId"),
		m_durationMillis(dict, "durationMillis")
	{}

	boost::python::dict							m_dict;
	DictionaryValue<std::string>				m_comment;
	DictionaryValue<std::string>				m_rating;
	DictionaryValue<std::vector<AlbumArt>>		m_albumArtRef;
	DictionaryValue<std::vector<std::string>>	m_artistId;
	DictionaryValue<std::string>				m_composer;
	DictionaryValue<int>						m_year;
	DictionaryValue<std::string>				m_creationTimestamp;
	DictionaryValue<std::string>				m_id;
	DictionaryValue<std::string>				m_album;
	DictionaryValue<int>						m_totalDiscCount;
	DictionaryValue<std::string>				m_title;
	DictionaryValue<std::string>				m_recentTimestamp;
	DictionaryValue<std::string>				m_albumArtist;
	DictionaryValue<int>						m_trackNumber;
	DictionaryValue<int>						m_discNumber;
	DictionaryValue<bool>						m_deleted;
	DictionaryValue<std::string>				m_nid;
	DictionaryValue<int>						m_totalTrackCount;
	DictionaryValue<std::string>				m_estimatedSize;
	DictionaryValue<std::string>				m_albumId;
	DictionaryValue<int>						m_beatsPerMinute;
	DictionaryValue<std::string>				m_genre;
	DictionaryValue<int>						m_playCount;
	DictionaryValue<std::vector<AlbumArt>>		m_artistArtRef;
	DictionaryValue<std::string>				m_kind;
	DictionaryValue<std::string>				m_artist;
	DictionaryValue<std::string>				m_lastModifiedTimestamp;
	DictionaryValue<std::string>				m_clientId;
	DictionaryValue<std::string>				m_durationMillis;
};

using SongRange = GeneratedRange<Song>;
using pyGeneratorToSongRangeConverter = pyGeneratorToGeneratedRangeConverter<Song>;

} // namespace GMusicApi