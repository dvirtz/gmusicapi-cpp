#pragma once

#include "utility.h"
MSC_DISABLE_WARNINGS
#include <boost/python.hpp>
MSC_RESTORE_WARNINGS
#include "AlbumArt.h"
#include "GeneratedRange.h"
#include <string>
#include <vector>

namespace GMusicApi
{

struct Song
{
	Song(const boost::python::dict& dict)
	{
		getFromDict(dict, "comment", m_comment);
		getFromDict(dict, "rating", m_rating);
		getFromDict(dict, "albumArtRef", m_albumArtRef);
		getFromDict(dict, "artistId", m_artistId);
		getFromDict(dict, "composer", m_composer);
		getFromDict(dict, "year", m_year);
		getFromDict(dict, "creationTimestamp", m_creationTimestamp);
		getFromDict(dict, "id", m_id);
		getFromDict(dict, "album", m_album);
		getFromDict(dict, "totalDiscCount", m_totalDiscCount);
		getFromDict(dict, "title", m_title);
		getFromDict(dict, "recentTimestamp", m_recentTimestamp);
		getFromDict(dict, "albumArtist", m_albumArtist);
		getFromDict(dict, "trackNumber", m_trackNumber);
		getFromDict(dict, "discNumber", m_discNumber);
		getFromDict(dict, "deleted", m_deleted);
		getFromDict(dict, "nid", m_nid);
		getFromDict(dict, "totalTrackCount", m_totalTrackCount);
		getFromDict(dict, "estimatedSize", m_estimatedSize);
		getFromDict(dict, "albumId", m_albumId);
		getFromDict(dict, "beatsPerMinute", m_beatsPerMinute);
		getFromDict(dict, "genre", m_genre);
		getFromDict(dict, "playCount", m_playCount);
		getFromDict(dict, "artistArtRef", m_artistArtRef);
		getFromDict(dict, "kind", m_kind);
		getFromDict(dict, "artist", m_artist);
		getFromDict(dict, "lastModifiedTimestamp", m_lastModifiedTimestamp);
		getFromDict(dict, "clientId", m_clientId);
		getFromDict(dict, "durationMillis", m_durationMillis);
	}

	std::string              m_comment;
	std::string              m_rating;
	std::vector<AlbumArt>    m_albumArtRef;
	std::vector<std::string> m_artistId;
	std::string              m_composer;
	int                      m_year;
	std::string              m_creationTimestamp;
	std::string              m_id;
	std::string              m_album;
	int                      m_totalDiscCount;
	std::string              m_title;
	std::string              m_recentTimestamp;
	std::string              m_albumArtist;
	int                      m_trackNumber;
	int                      m_discNumber;
	bool                     m_deleted;
	std::string              m_nid;
	int                      m_totalTrackCount;
	std::string              m_estimatedSize;
	std::string              m_albumId;
	int                      m_beatsPerMinute;
	std::string              m_genre;
	int                      m_playCount;
	std::vector<AlbumArt>    m_artistArtRef;
	std::string              m_kind;
	std::string              m_artist;
	std::string              m_lastModifiedTimestamp;
	std::string              m_clientId;
	std::string              m_durationMillis;
};

using SongRange = GeneratedRange<Song>;
using pyGeneratorToSongRangeConverter = pyGeneratorToGeneratedRangeConverter<Song>;

} // namespace GMusicApi